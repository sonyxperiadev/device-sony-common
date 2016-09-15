/*
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program;if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  *  Copyright (C) 2009-2015 Broadcom Corporation
  *  Copyright (C) 2015 Sony Mobile Communications Inc.
  *
  *  NOTE: This file has been modified by Sony Mobile Communications Inc.
  *  Modifications are licensed under the License.
 */

/************************************************************************************
*
*  Filename:      brcm_hci_dump.c
*
*  Description:   Client program to read hcisnoop packets from Line discipline driver
*
***********************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cutils/log.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include "utils.h"
#include "btsnoop.h"
#include "brcm_hci_dump.h"

#define DBG FALSE

#if DBG
#define BRCM_HCI_DUMP_DBG(fmt, arg...) ALOGI("brcm_hci_dump:%s "fmt"\n" ,__func__, ## arg)
#else
#define BRCM_HCI_DUMP_DBG(fmt, arg...)
#endif
#define BRCM_HCI_DUMP_ERR(fmt, arg...) ALOGE("brcm_hci_dump:%s "fmt"\n" ,__func__, ## arg)


/* netlink protocol type for which kernel will respond.
** This is also declared in Line discipline driver for Kernel space - User space
** communication. (For HCI packet to transfer from Kernel space to User space) */
#define NETLINK_USER 29

/* Size of memory allocated in kernel to userspace netlink socket to hold incoming hcisnoop packet */
#define MAX_PAYLOAD 2048

/* snoop status */
HCI_SNOOP_STATUS snoop_status = HCI_SNOOP_STOP;

/* Thread for hci snoop */
pthread_t thread_hcisnoop;

/* hci snoop filepath */
extern char hci_snoop_path[HCI_SNOOP_PATH_LEN];


/*******************************************************************************
**
** Function         acl_rx_frame_integrity_check
**
** Description      This function is called from the HCI transport when a
**                  L2CAP start packet has been received:
**                  - If the packet is the first message of a series of L2CAP
**                    fragmented frames:
**                      > Allocate a new buffer bigger enough to hold entire
**                        contents of all fragmented messages.
**                      > Copy content of this packet into new buffer
**                      > Keep the new buffer in a processing queue
**                  - If the packet is a L2CAP continuation packet
**                      > Copy the content into the base buffer
**                      > Checks if the updated base buffer has complete L2CAP
**                        message, i.e. no more continuation packets are
**                        expected.
**
** Returns          the address of a self-contained valid packet, or
**                  NULL if a fragmented packet or invalid packet
**
*******************************************************************************/


HC_BT_HDR* acl_rx_frame_integrity_check_v4l2 (HC_BT_HDR *p_rcv_msg)
{
    uint8_t     *p;
    uint16_t    handle;
    uint16_t    hci_len;
    uint16_t    total_len;
    uint16_t    cid;
    uint8_t     pkt_type;
    HC_BT_HDR  *p_return_buf = NULL;

    p = (uint8_t *)(p_rcv_msg + 1);
    total_len = 0;
    cid = 0;

    STREAM_TO_UINT16 (handle, p);
    STREAM_TO_UINT16 (hci_len, p);

    pkt_type = (uint8_t)(((handle) >> 12) & 0x0003);
    handle   = (uint16_t)((handle) & 0x0FFF);

    if ((pkt_type == ACL_RX_PKT_START) && (hci_len))
    {
        BRCM_HCI_DUMP_DBG("Inside pkt_type == ACL_RX_PKT_START && (hci_len)");
        /* Start Packet */
        STREAM_TO_UINT16 (total_len, p);
        STREAM_TO_UINT16 (cid, p);

        BRCM_HCI_DUMP_DBG("total_len = 0x%02X cid = 0x%02X", total_len, cid);
    }

    if (utils_get_count())
    {
        uint16_t save_handle;
        HC_BT_HDR *p_hdr = utils_get_first();

        BRCM_HCI_DUMP_DBG("In acl_rx_q.count");

        while (p_hdr != NULL)
        {
            p = (uint8_t *)(p_hdr + 1);
            STREAM_TO_UINT16 (save_handle, p);
            save_handle   = (uint16_t)((save_handle) & 0x0FFF);
            if (save_handle == handle)
            {
                p_return_buf = p_hdr;
                break;
            }
            p_hdr = (HC_BT_HDR *) utils_getnext(p_hdr);
        }
    }

    if (pkt_type == ACL_RX_PKT_START)       /*** START PACKET ***/
    {
        BRCM_HCI_DUMP_DBG("Start of packet");
        /* Start of packet. If we were in the middle of receiving */
        /* a packet on the same ACL handle, the original packet is incomplete.
         * Drop it. */
        if (p_return_buf)
        {
            BRCM_HCI_DUMP_DBG("HCI_V4L2 - dropping incomplete ACL frame");

            utils_remove_from_queue(p_return_buf);
            utils_release(p_return_buf);

            p_return_buf = NULL;
        }

        if (hci_len)
        {
            BRCM_HCI_DUMP_DBG("hci_len = %d", hci_len);
            if (cid == 0)
            {
                BRCM_HCI_DUMP_DBG("HCI_V4L2 - dropping ACL frame with" \
                    "invalid CID 0x0000.");
                return NULL;
            }

            if (total_len > (uint16_t)MAX_ACL_PDU_SIZE)
            {
                BRCM_HCI_DUMP_DBG("HCI_V4L2 - dropping ACL frame with "\
                    "PDU size 0x%04x exceeding max design limit.", total_len);
                return NULL;
            }

            if (hci_len > (uint16_t)(MAX_ACL_PDU_SIZE+L2CAP_HEADER_SIZE))
            {
                BRCM_HCI_DUMP_DBG("HCI_V4L2 - dropping ACL frame with" \
                 "HCI packet size 0x%04x exceeding max design limit.", hci_len);
                return NULL;
            }

            if (hci_len > (total_len + L2CAP_HEADER_SIZE))
            {
                BRCM_HCI_DUMP_DBG("HCI_V4L2 - dropping ACL frame with"\
                    "payload size 0x%04x bigger than its carried PDU +"\
                    "header size 0x%04x", hci_len,
                        (total_len + L2CAP_HEADER_SIZE));
                return NULL;
            }

            if ((total_len + L2CAP_HEADER_SIZE) > hci_len)
            {
                /* Will expect to see fragmented ACL packets */
                BRCM_HCI_DUMP_DBG("Will expect to see fragmented ACL packets");

                /* Allocate a buffer for message */
                int len = total_len + HCI_ACL_PREAMBLE_SIZE + \
                                L2CAP_HEADER_SIZE + BT_HC_HDR_SIZE;
                BRCM_HCI_DUMP_DBG("allocate a buffer for message len = %d", len);
                p_return_buf = (HC_BT_HDR *) utils_alloc(len);
                BRCM_HCI_DUMP_DBG("allocated memory for p_return_buf=%p",
                                      p_return_buf);

                if (p_return_buf)
                {
                    p_return_buf->offset = 0;
                    p_return_buf->layer_specific = 0;
                    p_return_buf->event = MSG_HC_TO_STACK_HCI_ACL;
                    p_return_buf->len = p_rcv_msg->len;
                    memcpy((uint8_t *)(p_return_buf+1), \
                            (uint8_t *)(p_rcv_msg+1),\
                            p_rcv_msg->len);

                    /* Keep the base buffer address in the watching queue */
                    BRCM_HCI_DUMP_DBG("Adding START pkt to acl_rx_q with "\
                                 "offset = %d len = %d",p_return_buf->offset,
                                                          p_return_buf->len);
                    utils_enqueue(p_return_buf);

                    BRCM_HCI_DUMP_DBG("Also writing START pkt to" \
                        "btsnoop_capture");
                    btsnoop_capture(p_return_buf, TRUE);
                }
                else
                {
                    BRCM_HCI_DUMP_DBG("HCI_V4L2 - failed to get buffer to "\
                        "assemble fragmented ACL packets with total length = "\
                        "0x%04x",
                          (total_len+L2CAP_HEADER_SIZE+HCI_ACL_PREAMBLE_SIZE));
                }
                return NULL;
            }
        }

        BRCM_HCI_DUMP_DBG("p_return_buf = p_rcv_msg;");
        p_return_buf = p_rcv_msg;
    }
    else                                    /*** CONTINUATION PACKET ***/
    {
        BRCM_HCI_DUMP_DBG("CONTINUATION PACKET");
        uint16_t rcv_len = hci_len;

        if (p_return_buf)
        {
            /* Packet continuation and found the original rx buffer */
            BRCM_HCI_DUMP_DBG("Packet continuation and found the original rx"\
                                                                     "buffer");
            uint8_t *p_f = p = (uint8_t *)(p_return_buf + 1) + 2;
            uint32_t new_len;

            STREAM_TO_UINT16 (hci_len, p);
            STREAM_TO_UINT16 (total_len, p); // the length of ACL packet

            BRCM_HCI_DUMP_DBG("CONTINUATION PACKET hci_len = 0x%04X " \
                "total_len = 0x%04X", hci_len, total_len);

            new_len = (uint32_t)hci_len + (uint32_t)rcv_len - L2CAP_HEADER_SIZE;

            BRCM_HCI_DUMP_DBG("CONTINUATION PACKET new_len = %d", new_len);

            if (new_len > (uint32_t) total_len)
            {
                BRCM_HCI_DUMP_DBG("HCI_V4L2 - ACL frames reassembly failed:");
                STREAM_TO_UINT16 (handle, p);
                BRCM_HCI_DUMP_DBG("\t Total length of all segmented packets exceeds"
                      "\t the original PDU length [CID:0x%04X].", handle);

                ALOGW("HCI_V4L2 - dropping invalid ACL frames");
                BRCM_HCI_DUMP_DBG("remove from queue");
                utils_remove_from_queue(p_return_buf);
                utils_release(p_return_buf);
                p_return_buf = NULL;
                return p_return_buf;
            }

            // Update HCI header of first segment (base buffer) with new len
            BRCM_HCI_DUMP_DBG("Update HCI header of first segment (base buffer)"\
                                                               "with new len");
            hci_len += rcv_len;
            UINT16_TO_STREAM (p_f, hci_len);
            BRCM_HCI_DUMP_DBG("hci_len = 0x%02X",hci_len);

            BRCM_HCI_DUMP_DBG("Copy content of this fragmented packet into"\
                                                             "the base buffer");
            // Copy content of this fragmented packet into the base buffer
            p_f = (uint8_t *)(p_return_buf + 1);
            p = (uint8_t *)(p_rcv_msg + 1);
            memcpy((uint8_t *)(p_f+p_return_buf->len), \
                    (uint8_t *)(p+HCI_ACL_PREAMBLE_SIZE), \
                    rcv_len);
            p_return_buf->len += rcv_len;
            BRCM_HCI_DUMP_DBG("p_f = %p p = %p rcv_len = 0x%02X "\
                "p_return_buf->len = 0x%02X", p_f, p, rcv_len, p_return_buf->len);
            BRCM_HCI_DUMP_DBG("Copy content of this fragmented packet into "\
                                                             "the base buffer");

            if (total_len > (p_return_buf->len -
                                  (HCI_ACL_PREAMBLE_SIZE+L2CAP_HEADER_SIZE)))
            {
                /* If the L2CAP length has not been reached, tell HCI_V4L2 not to send
                            * the fragmented packet to stack */
                BRCM_HCI_DUMP_DBG("If the L2CAP length has not been reached,"\
                                          "tell HCI_V4L2 not to send" \
                                          "the fragmented packet to stack");
                return NULL;
            }
            else
            {
                /*
                 * All fragmented packets had been reassembled.
                 * Remove the base buffer from the watching list.
                 */
                BRCM_HCI_DUMP_DBG("All fragmented packets had been reassembled");
                utils_remove_from_queue(p_return_buf);
                BRCM_HCI_DUMP_DBG("after utils_remove_from_queue");
            }
        }
        else
        {
            BRCM_HCI_DUMP_DBG("HCI_V4L2 - dropping ACL frame marked as "\
                "CONTINUING Pkt but found no START Pkt");
            return NULL;
        }
    }

    BRCM_HCI_DUMP_DBG("p_return_buf->len = %d at the end", p_return_buf->len);
    BRCM_HCI_DUMP_DBG("writing to snoop file when at end");

    BRCM_HCI_DUMP_DBG("write to btsnoop_capture with p_return_buf->len = %d",
                                                             p_return_buf->len);
    btsnoop_capture(p_return_buf, TRUE);
    return (p_return_buf);
}



/*****************************************************************************
**   Socket signal functions to wake up hci_snoop_thread for termination
**
**   creating an unnamed pair of connected sockets
**      - signal_fds[0]: join fd_set in select call of userial_read_thread
**      - signal_fds[1]: trigger from userial_close
*****************************************************************************/
static int signal_fds[2]={0,1};

static inline int create_signal_fds(fd_set* set)
{
    if(signal_fds[0]==0 && socketpair(AF_UNIX, SOCK_STREAM, 0, signal_fds)<0)
    {
        BRCM_HCI_DUMP_ERR("create_signal_sockets:socketpair failed, errno: %d",
                                                                        errno);
        return -1;
    }
    FD_SET(signal_fds[0], set);
    return signal_fds[0];
}

static inline int send_wakeup_signal(char sig_cmd)
{
    return send(signal_fds[1], &sig_cmd, sizeof(sig_cmd), 0);
}

static inline char reset_signal()
{
    char sig_recv = -1;
    recv(signal_fds[0], &sig_recv, sizeof(sig_recv), MSG_WAITALL);
    return sig_recv;
}

static inline int is_signaled(fd_set* set)
{
    return FD_ISSET(signal_fds[0], set);
}


/* Read thread for snooping packets from Line discipline driver */
static void* v4l2_hci_snoop_thread(void* parameters)
{
    HC_BT_HDR *p_buf = NULL;
    fd_set input;
    char reason = 0;
    int len = 0;

    //ACL related variables
    uint8_t *p;
    uint8_t byte;
    uint16_t msg_len;

    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    int sock_fd;
    struct msghdr msg;

    BRCM_HCI_DUMP_DBG("hci_snoop_thread START");
    snoop_status = HCI_SNOOP_RUNNING;

    sock_fd = socket(PF_NETLINK, SOCK_RAW , NETLINK_USER);
    if (sock_fd < 0)
    {
        BRCM_HCI_DUMP_ERR("Unable to create netlink socket");
        return -1;
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); /* self pid */

    bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    strcpy(NLMSG_DATA(nlh), "START HCI SNOOP");

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;


    BRCM_HCI_DUMP_DBG("Sending message to Line discipline driver\n");
    sendmsg(sock_fd, &msg, 0);

    BRCM_HCI_DUMP_DBG("Waiting for response from Line discipline driver\n");

    fcntl(sock_fd, F_SETFL, O_NONBLOCK);
    BRCM_HCI_DUMP_DBG("set netlink socket to non-blocking");

    utils_init();

    /* Receive packet from Line discipline driver */
    while(1)
    {
        /* initialize the fdset */
        /* Initialize the input fd set */
        FD_ZERO(&input);
        FD_SET(sock_fd, &input);

        int fd_max = create_signal_fds(&input);
        fd_max = fd_max > sock_fd ? fd_max : sock_fd;

        /* select call will block until snoop pkt is received from ldisc */
        len = select(fd_max+1, &input, NULL, NULL, NULL);
        if(is_signaled(&input))
        {
            BRCM_HCI_DUMP_DBG("HCI snoop thread is signalled");
            reason = reset_signal();
            BRCM_HCI_DUMP_DBG("reason = %d", reason);
            if (reason == HCISNOOP_EXIT)
            {
                BRCM_HCI_DUMP_DBG("HCI snoop thread termination SIGNAL RECEIVED");
                snoop_status = HCI_SNOOP_STOP;
                free(nlh);
                close(sock_fd);
                BRCM_HCI_DUMP_DBG("Exiting snoop thread");
                return 0;
            }
            else {
                BRCM_HCI_DUMP_ERR("UNKNOWN signal received");
            }
        }

        if (FD_ISSET(sock_fd, &input))
        {
            recvmsg(sock_fd, &msg, 0);
            BRCM_HCI_DUMP_DBG("recevied packet");
            len = NLMSG_PAYLOAD(nlh, 0);
            BRCM_HCI_DUMP_DBG("finished reading len of recevied packet");


            if(len > 0) {
                BRCM_HCI_DUMP_DBG("Received packet of len=%d from ldisc", len);
                p_buf = NLMSG_DATA(nlh);

                if(p_buf == NULL) {
                    BRCM_HCI_DUMP_DBG("p_buf is NULL");
                    continue;
                }

                /* start processing the Rx packets */
                    if(p_buf->event == MSG_HC_TO_STACK_HCI_ACL)
                    {
                        BRCM_HCI_DUMP_DBG("ACL packet");
                        /* ACL packets */
                        HC_BT_HDR *p_msg;

                        if (p_buf->len < HCI_ACL_PREAMBLE_SIZE)
                        {
                            BRCM_HCI_DUMP_ERR("[h4] Invalid ACL length "\
                                              "(0x%04x) drop this packet", \
                                                                    p_buf->len);
                            continue;
                        }

                        p = (uint8_t *) (p_buf + 1);
                        byte = *(p+1);
                        p += 2;
                        STREAM_TO_UINT16(msg_len, p);
                        BRCM_HCI_DUMP_DBG("ACL packet: msg_len = %d", msg_len);

                        if (p_buf->len != (msg_len + HCI_ACL_PREAMBLE_SIZE))
                        {
                            BRCM_HCI_DUMP_ERR("[h4] ACL message length "\
                                "(0x%04x) does not match to its data payload" \
                                "length (0x%04x) drop this packet",
                                  p_buf->len, msg_len);
                            BRCM_HCI_DUMP_DBG("calling continue");
                            continue;
                        }

                        if (msg_len)
                        {
                            BRCM_HCI_DUMP_DBG("check if start of packet");
                            /* Check if this is a start packet */
                            byte = ((byte >> 4) & 0x03);

                            if (byte == ACL_RX_PKT_START)
                            {
                                BRCM_HCI_DUMP_DBG("byte == ACL_RX_PKT_START");
                                if (msg_len < L2CAP_HEADER_SIZE)
                                {
                                    BRCM_HCI_DUMP_ERR("[h5] dropping "\
                                        "incomplete ACL frame with data" \
                                       "payload len=%d (<L2CAP_HEADER_SIZE).",
                                        msg_len);
                                    BRCM_HCI_DUMP_DBG("calling continue");
                                    continue;
                                }
                            }
                        }

                        if ((p_msg = acl_rx_frame_integrity_check_v4l2(p_buf))
                                                                       == NULL)
                        {
                            BRCM_HCI_DUMP_DBG("calling continue");
                            continue;
                        }
                        else {
                            if(p_msg != p_buf)
                            {
                                BRCM_HCI_DUMP_DBG("freeing p_msg");
                                utils_release(p_msg);
                            }
                        }

                        BRCM_HCI_DUMP_DBG("ACL packet: p_msg->len = %d",
                                                                   p_msg->len);
                    }
                    /* For non Evnt pkt */
                    else if (p_buf->event == MSG_HC_TO_STACK_HCI_EVT)
                    {
                        BRCM_HCI_DUMP_DBG("EVT packet");
                        /* Event packets */
                        if (p_buf->len < HCI_EVT_PREAMBLE_SIZE)
                        {
                            BRCM_HCI_DUMP_ERR("[h5] Invalid EVT length " \
                                "(0x%04x) drop this packet", p_buf->len);
                            continue;
                        }

                        p = (uint8_t *) (p_buf + 1);
                        msg_len = *(p+1);

                        if (p_buf->len != (msg_len + HCI_EVT_PREAMBLE_SIZE))
                        {
                            BRCM_HCI_DUMP_ERR("[h4] EVT message length "\
                                "(0x%04x) does not match to " \
                                  "its parameters total length (0x%04x) drop" \
                                  "this packet", p_buf->len, msg_len);
                            continue;
                        }

                        btsnoop_capture(p_buf, TRUE);
                    }
                /* start processing the Tx packets */
                    else if(p_buf->event == MSG_STACK_TO_HC_HCI_ACL ||
                            p_buf->event == MSG_STACK_TO_HC_HCI_SCO ||
                            p_buf->event == MSG_STACK_TO_HC_HCI_CMD ||
                            p_buf->event == MSG_FM_TO_HC_HCI_CMD)
                    {
                        btsnoop_capture(p_buf, FALSE);
                    }
                    else if (p_buf->event == MSG_HC_TO_FM_HCI_EVT){
                        btsnoop_capture(p_buf, TRUE);
                    }
                /* UNKNOWN packets */
                    else
                    {
                        BRCM_HCI_DUMP_ERR("UNKNOWN event for the packet");
                    }
             }
        }
        else {
            BRCM_HCI_DUMP_ERR("select failed with len=%d", len);
        }
    }

    BRCM_HCI_DUMP_DBG("Exiting thread hci_snoop_thread from end");

    return 0;
}


int hci_snoop_bkp_file()
{
    if (hci_snoop_path != NULL)
    {
            char new_path[256] = {0};

            strcpy(new_path, hci_snoop_path);
            strcat(new_path, ".old");
            BRCM_HCI_DUMP_DBG("renaming file and creating backup");
            if (rename(hci_snoop_path, new_path))
            {
                BRCM_HCI_DUMP_ERR("logging():rename failed, %d", errno);
            }
            BRCM_HCI_DUMP_DBG("opening snoop file");
            btsnoop_open(hci_snoop_path);
            return 0;
    }
    else {
        BRCM_HCI_DUMP_ERR("snoop file path is NULL");
        return -1;
    }

}

int v4l2_start_hci_snoop()
{
    int result=0;

    if(snoop_status == HCI_SNOOP_STOP)
    {
        btsnoop_init();

        /* backup previous file and open new snoop file */
        if(hci_snoop_bkp_file())
        {
            BRCM_HCI_DUMP_ERR("Unable to create backup file");
            return -1;
        }

        /* start hci snoop thread */
        if ((result=pthread_create(&thread_hcisnoop, NULL,
                                          &v4l2_hci_snoop_thread, NULL)) < 0)
        {
            BRCM_HCI_DUMP_ERR("pthread_create() FAILED result:%d", result);
            btsnoop_close();
            btsnoop_cleanup();
            return result;
        }

        ALOGI("brcm_hci_dump:%s hcisnoop read thread STARTED", __func__);
    }
    else {
        BRCM_HCI_DUMP_DBG("hcisnoop read thread already RUNNING");
    }
    return result;
}

int v4l2_stop_hci_snoop()
{
    int result;

    BRCM_HCI_DUMP_DBG("called");

    if (snoop_status == HCI_SNOOP_RUNNING)
    {
        BRCM_HCI_DUMP_DBG("Sending signal HCISNOOP_EXIT");
        send_wakeup_signal(HCISNOOP_EXIT);
    }

    BRCM_HCI_DUMP_DBG("Waiting for pthread_join");
    if ((result=pthread_join(thread_hcisnoop, NULL)) < 0) {
        ALOGE("pthread_join() FAILED result:%d", result);
        return result;
    }

    BRCM_HCI_DUMP_DBG("snoop joined");

    btsnoop_close();
    btsnoop_cleanup();

    BRCM_HCI_DUMP_DBG("btsnoop cleanup done");

    BRCM_HCI_DUMP_DBG("hcisnoop read thread EXITED");
    return result;
}

int v4l2_get_hci_snoop_status()
{
    return snoop_status;
}




