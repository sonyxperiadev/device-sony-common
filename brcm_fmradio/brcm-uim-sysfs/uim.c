/*
 *  User Mode Init manager - For shared transport
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
*  Filename:      uim.c
*
*  Description:   User Interface Module (UIM) for performing UART config when a driver (BT protocol driver or
*                      V4L2 based FM driver) registers with Line discipline driver.
*                      This program runs as a daemon on system bootup.
*
***********************************************************************************/
/* Set macro to true in buildspec.mk */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <poll.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#include "uim.h"
#include "brcm_hci_dump.h"
#include "v4l2_cfg.h"
#ifdef ANDROID
#include <private/android_filesystem_config.h>
#include <cutils/properties.h>
#include <cutils/log.h>
#include <cutils/misc.h>
#endif

#define LOG_TAG "brcm-uim"

#define UIM_DEBUG 1

#if UIM_DEBUG          /* limited debug messages */
#define UIM_START_FUNC()      ALOGI("brcm-uim: Begin %s", __FUNCTION__)
#define UIM_END_FUNC()      ALOGI("brcm-uim: End %s", __FUNCTION__)
#define UIM_DBG(fmt, arg...)  ALOGI("brcm-uim:"fmt"\n" , ## arg)
#else /* error msgs only */
#define UIM_START_FUNC()
#define UIM_END_FUNC()
#define UIM_DBG(fmt, arg...)
#endif
#define UIM_VER(fmt, arg...) ALOGI("brcm-uim: "fmt"\n" , ## arg)
#define UIM_ERR(fmt, arg...) ALOGE("brcm-uim: ERR "fmt"\n" , ## arg)


#define CFG_PARAM_STRING_SIZE 300
#define UIM_FAIL -1
#define FACTORY_BT_BDADDR_STORAGE_LEN   17
#define BD_ADDR_LEN     6                   /* Device address length */
#define PROPERTY_BT_BDADDR_PATH         "ro.bt.bdaddr_path"
#define PERSIST_BDADDR_PROPERTY         "persist.service.bdroid.bdaddr"
#define STACK_CONF_FILE "/etc/bluetooth/bt_stack.conf"
#define FW_PATCH_FILENAME_MAXLEN 80
#define fw_patchfile_path "/system/etc/firmware"
#define FW_PATCHFILE_EXTENSION      ".hcd"
#define FW_PATCHFILE_EXTENSION_LEN  4
#define HCI_EVT_CMD_CMPL_LOCAL_NAME_STRING 7
#define READ_LOCALNAME_RESP_BUFF_SIZE 100

typedef char bdstr_t[18];

/* Maintains the exit state of UIM*/
static int exiting;

/* File descriptor for the UART device*/
int dev_fd = -1;

/* termios to set baudrate */
struct termios termios;

/* BD address as string and a pointer to array of hex bytes */
bdaddr_t bd_addr;

/* parameters read from bt_vendor.conf */
static char hw_cfg_string[CFG_PARAM_STRING_SIZE] = {0}; /* pass as parameter to ldisc */
static unsigned long cust_baud_rate = 3000000;
static char driver_module_path[MAX_KMODULE_PATH_SIZE] = "/system/lib/modules/";
static char uart_port_name[UART_PORT_NAME_SIZE] = "/dev/ttyHS0";
int lpmenable;
int hci_snoop_enable = 0;
char hci_snoop_path[HCI_SNOOP_PATH_LEN] = "/sdcard/btsnoop_hci.log";
static char bt_dbg_cfg_string[CFG_PARAM_STRING_SIZE] = "";
static char fm_dbg_cfg_string[CFG_PARAM_STRING_SIZE] = "";
static char fw_patchfile_name[FW_PATCH_FILENAME_MAXLEN] = "";


btuim_lpm_param_t lpm_uim_param =
{
    LPM_SLEEP_MODE,//1
    LPM_IDLE_THRESHOLD,//1
    LPM_HC_IDLE_THRESHOLD,//1
    LPM_BT_WAKE_POLARITY,//1
    LPM_HOST_WAKE_POLARITY,//1
    LPM_ALLOW_HOST_SLEEP_DURING_SCO,//1
    LPM_COMBINE_SLEEP_MODE_AND_LPM,//1
    LPM_ENABLE_UART_TXD_TRI_STATE,//0
    0,  /* not applicable */
    0,  /* not applicable */
    0,  /* not applicable */
    LPM_PULSED_HOST_WAKE//0
};

int pass_vendor_params()
{
    int fd_vendor_params = 0;
    int len;

    fd_vendor_params = open(LDISC_VENDOR_PARAMS, O_WRONLY);
    if (fd_vendor_params > 0)
    {
        if(write(fd_vendor_params, hw_cfg_string, CFG_PARAM_STRING_SIZE) < 0)
            return UIM_FAIL;

        UIM_DBG("vendor params passed to ldisc");
        return 0;
    }
    else
        return UIM_FAIL;
}

/*******************************************************************************
**
** Function        userial_set_port
**
** Description     Configure UART port name
**
** Returns         0 : Success
**                 Otherwise : Fail
**
*******************************************************************************/
int userial_set_port(char *p_conf_name, char *p_conf_value)
{
    strcpy(uart_port_name, p_conf_value);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}
/*******************************************************************************
**
** Function        hw_set_lpm_polarity
**
** Description     configures host & bt wake polarity
**                 0: active low (negative logic)
**                 1: active high (positive logic)
**
** Returns         0 : Success
**                 Otherwise : Fail
**
*******************************************************************************/
int hw_set_lpm_polarity(char *p_conf_name, char *p_conf_value)
{
    uint8_t polarity = (atoi(p_conf_value) & 1);
    lpm_uim_param.bt_wake_polarity = lpm_uim_param.host_wake_polarity = polarity;

    UIM_DBG("%s BT_WAKE and HOST_WAKE polarity set", __func__);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}
/*****************************************************************************
* Function to convert the BD address from hex to ascii value
*****************************************************************************/

char* bd2str(const bdaddr_t *bdaddr, bdstr_t *bdstr)
    {
        const uint8_t *addr = bdaddr->address;

        sprintf(*bdstr, "%02x:%02x:%02x:%02x:%02x:%02x",
                           addr[0], addr[1], addr[2],
                           addr[3], addr[4], addr[5]);
        return *bdstr;
}
/*******************************************************************************
**
** Function        hw_set_btwake
**
** Description     Enable bluesleep in Line discipline driver based on this value
**
** Returns         0 : Success
**                 Otherwise : Fail
**
*******************************************************************************/
int hw_set_btwake(char *p_conf_name, char *p_conf_value)
{
    char bluesleep_enable[10];
    int bluesleepenable;
    strcat(hw_cfg_string, " LpmUseBluesleep=");
    if (strcmp(p_conf_value, "true") == 0){
        bluesleepenable =1;
        sprintf(bluesleep_enable,"%d",bluesleepenable);
    }
    else{
        bluesleepenable =0;
        sprintf(bluesleep_enable,"%d",bluesleepenable);
    }
    strcat(hw_cfg_string, bluesleep_enable);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}

/*******************************************************************************
 **
 ** Function        hw_check_readcontroller_addr
 **
 ** Description     Check whether to read controller bd address
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int hw_check_readcontroller_addr(char *p_conf_name, char *p_conf_value)
{
    char controller_addrread[10];
    int controlleraddrread;
    strcat(hw_cfg_string, " ControllerAddrRead=");
    if (strcmp(p_conf_value, "true") == 0){
        controlleraddrread =1;
        sprintf(controller_addrread,"%d",controlleraddrread);
    }
    else{
        controlleraddrread =0;
        sprintf(controller_addrread,"%d",controlleraddrread);
    }
    strcat(hw_cfg_string, controller_addrread);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}

/*******************************************************************************
 **
 ** Function        hw_set_lpm
 **
 ** Description     Enable LPM in Line discipline driver based on this value
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int hw_set_lpm(char *p_conf_name, char *p_conf_value)
{
    if (strcmp(p_conf_value, "true") == 0)
        lpmenable =1;
    else
        lpmenable =0;

    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}


/*******************************************************************************
 **
 ** Function        hw_set_uart_baudrate
 **
 ** Description     Give the baudrate for uart
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int hw_set_uart_baudrate(char *p_conf_name, char *p_conf_value)
{
    strcat(hw_cfg_string, " custom_baudrate=");
    strcat(hw_cfg_string, p_conf_value);
    sscanf(p_conf_value, "%lu", &cust_baud_rate);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}


/*******************************************************************************
 **
 ** Function        hw_set_driver_module_path
 **
 ** Description     set the location of .ko modules (driver modules) to insmod at startup.
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int hw_set_driver_module_path(char *p_conf_name, char *p_conf_value)
{
    strcpy(driver_module_path, p_conf_value);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}

/*******************************************************************************
 **
 ** Function        hw_set_patchram_settlement_delay
 **
 ** Description     set the location of .ko modules (driver modules) to insmod at startup.
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int hw_set_patchram_settlement_delay(char *p_conf_name, char *p_conf_value)
{
    strcat(hw_cfg_string, " patchram_settlement_delay=");
    strcat(hw_cfg_string, p_conf_value);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}

/*******************************************************************************
 **
 ** Function        hw_set_patchram_filename
 **
 ** Description     set the patchram filename.
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int hw_set_patchram_filename(char *p_conf_name, char *p_conf_value)
{
    strcpy(fw_patchfile_name, p_conf_value);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}


#if DBG_V4L2_DRIVERS
/*******************************************************************************
 **
 ** Function        dbg_ldisc_drv
 **
 ** Description     set to enable debugging in Line discipline driver
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int dbg_ldisc_drv(char *p_conf_name, char *p_conf_value)
{
    strcat(hw_cfg_string, " ldisc_dbg_param=");
    strcat(hw_cfg_string, p_conf_value);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}

/*******************************************************************************
 **
 ** Function        dbg_bt_drv
 **
 ** Description     set to enable debugging in BT driver
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int dbg_bt_drv(char *p_conf_name, char *p_conf_value)
{
    strcat(bt_dbg_cfg_string, " bt_dbg_param=");
    strcat(bt_dbg_cfg_string, p_conf_value);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}

/*******************************************************************************
 **
 ** Function        dbg_fm_drv
 **
 ** Description     set to enable debugging in FM driver
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int dbg_fm_drv(char *p_conf_name, char *p_conf_value)
{
    strcat(fm_dbg_cfg_string, " fm_dbg_param=");
    strcat(fm_dbg_cfg_string, p_conf_value);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}

#endif


#if V4L2_SNOOP_ENABLE
/*******************************************************************************
 **
 ** Function        enable_hci_snoop
 **
 ** Description     read parameter to enable/disable hci snoop for V4L2
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int enable_hci_snoop(char *p_conf_name, char *p_conf_value)
{
    if (strcmp(p_conf_value, "true") == 0)
    {
        hci_snoop_enable = 1;
        strcat(hw_cfg_string, " ldisc_snoop_enable_param=1");
    }
    else
    {
        hci_snoop_enable = 0;
        strcat(hw_cfg_string, " ldisc_snoop_enable_param=0");
    }
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}


/*******************************************************************************
 **
 ** Function        path_hci_snoop
 **
 ** Description     read filepath for hci snoop for V4L2
 **
 ** Returns         0 : Success
 **                 Otherwise : Fail
 **
 *******************************************************************************/
int path_hci_snoop(char *p_conf_name, char *p_conf_value)
{
    memset(hci_snoop_path, 0, sizeof(hci_snoop_path));
    strcpy(hci_snoop_path, p_conf_value);
    UIM_DBG("%s = %s", p_conf_name, p_conf_value);
    return 0;
}
#endif


/******************************************************************************
**  Local type definitions
******************************************************************************/

#define CONF_COMMENT '#'
#define CONF_DELIMITERS " =\n\r\t"
#define CONF_VALUES_DELIMITERS "=\n\r\t"
#define CONF_MAX_LINE_LEN 255

typedef int (conf_action_t)(char *p_conf_name, char *p_conf_value);

typedef struct {
    const char *conf_entry;
    conf_action_t *p_action;
} conf_entry_t;


/*
 * Currently supported entries in vendor_conf and corresponding action functions
 */
static const conf_entry_t vendor_conf_table[] = {
    {"UartPort", userial_set_port},
    {"UartBaudRate", hw_set_uart_baudrate},
    {"FwPatchSettlementDelay", hw_set_patchram_settlement_delay},
    {"LpmWakePolarity",hw_set_lpm_polarity},
    {"DriverModulePath", hw_set_driver_module_path},
    {"LpmEnable", hw_set_lpm},
    {"LpmUseBluesleep", hw_set_btwake},
    {"UseControllerBdaddr",hw_check_readcontroller_addr},
    {"FwPatchFileName", hw_set_patchram_filename},
#if DBG_V4L2_DRIVERS
    {"DBG_BT_DRV",dbg_bt_drv},
    {"DBG_LDISC_DRV",dbg_ldisc_drv},
    {"DBG_FM_DRV",dbg_fm_drv},
#endif
    {(const char *) NULL, NULL}
};


#if V4L2_SNOOP_ENABLE
static const conf_entry_t stack_conf_table[] = {
    {"BtSnoopLogOutput", enable_hci_snoop},
    {"BtSnoopFileName", path_hci_snoop},
    {(const char *) NULL, NULL}
};
#endif


/*******************************************************************************
**
** Function        vnd_load_conf
**
** Description     Read conf entry from p_path file one by one and call
**                 the corresponding config function
**
** Returns         0 on success
**                   -1 on failure
**
*******************************************************************************/
int vnd_load_conf(const char *p_path, const conf_entry_t* conf_table)
{
    FILE          *p_file;
    char          *p_name;
    char          *p_value;
    conf_entry_t  *p_entry;
    char line[CONF_MAX_LINE_LEN+1]; /* add 1 for \0 char */

    UIM_START_FUNC();
    UIM_DBG("Attempt to load conf from %s", p_path);

    if ((p_file = fopen(p_path, "r")) != NULL)
    {
        /* read line by line */
        while (fgets(line, CONF_MAX_LINE_LEN+1, p_file) != NULL)
        {
            if (line[0] == CONF_COMMENT)
                continue;

            p_name = strtok(line, CONF_DELIMITERS);

            if (NULL == p_name)
            {
                continue;
            }

            p_value = strtok(NULL, CONF_DELIMITERS);

            if (NULL == p_value)
            {
                UIM_DBG("vnd_load_conf: missing value for name: %s", p_name);
                continue;
            }

            p_entry = (conf_entry_t *)conf_table;

            while (p_entry->conf_entry != NULL)
            {
                if (strcmp(p_entry->conf_entry, (const char *)p_name) == 0)
                {
                    p_entry->p_action(p_name, p_value);
                    break;
                }

                p_entry++;
            }
        }

        fclose(p_file);
        UIM_END_FUNC();
        return 0;
    }
    else
    {
        UIM_DBG("vnd_load_conf file >%s< not found", p_path);
        UIM_END_FUNC();
        return -1;
    }
    return 0;
}


static inline void cleanup()
{
    if (dev_fd == -1)
        return;

    tcflush(dev_fd, TCIOFLUSH);

    close(dev_fd);
    dev_fd = -1;

    UIM_DBG("%s complete", __func__);
}

static inline void err_cleanup(int st_fd)
{
    cleanup();
    UIM_ERR("setting upio power to 0 for error recovery");
    upio_set_bluetooth_power(0);
    UIM_ERR("Closing shared transport fd - st_fd");
    if (st_fd)
        close(st_fd);
    st_fd = -1;
    UIM_ERR("Restarting UIM due to error!");
}


#ifdef ANDROID   /* library for android to do insmod/rmmod  */

/*****************************************************************************/
/* Function to insert the kernel module into the system
  ****************************************************************************/
static int insmod(const char *filename, const char *args)
{
    void *module;
    unsigned int size;
    int ret = -1;

    UIM_START_FUNC();

    module = (void *)load_file(filename, &size);
    if (!module)
    {
        UIM_DBG("unable to access %s", filename);
        return ret;
    }

    ret = init_module(module, size, args);
    free(module);

    UIM_DBG("%s err code = %d",__func__, ret);
    return ret;
}


/*****************************************************************************/
/* Function to remove the kernel module from the system
****************************************************************************/
static int rmmod(const char *modname)
{
    int ret = -1;
    int maxtry = MAX_TRY;

    UIM_START_FUNC();

    /* Retry MAX_TRY number of times in case of failure */
    while (maxtry-- > 0) {
        ret = delete_module(modname, O_NONBLOCK | O_EXCL);
        if (ret < 0 && errno == EAGAIN)
            sleep(1);
        else
            break;
    }

    /* Failed to remove the module */
    if (ret != 0)
        UIM_ERR("Unable to unload driver module \"%s\": %s",
                modname, strerror(errno));
    return ret;
}
#endif /* ANDROID */



/*****************************************************************************/
/* Function to read the HCI event from the given file descriptor
 *
 * This will parse the response received and returns error
 * if the required response is not received
 */
int read_hci_event(int fd, unsigned char *buf, int size)
{
    int remain, rd;
    int count = 0;
    int reading = 1;
    int rd_retry_count = 0;
    struct timespec tm = {0, 50*1000*1000};

    UIM_START_FUNC();

    UIM_VER(" read_hci_event");
    if (size <= 0)
        return -1;

    /* The first byte identifies the packet type. For HCI event packets, it
     * should be 0x04, so we read until we get to the 0x04. */
    while (reading) {
        rd = read(fd, buf, 1);
        if (rd <= 0 && rd_retry_count++ < 4) {
            nanosleep(&tm, NULL);
            continue;
        } else if (rd_retry_count >= 4) {
            return -1;
        }

        if (buf[0] == RESP_PREFIX) {
            break;
        }
    }
    count++;

    /* The next two bytes are the event code and parameter total length. */
    while (count < 3) {
        rd = read(fd, buf + count, 3 - count);
        if (rd <= 0)
            return -1;
        count += rd;
    }

    /* Now we read the parameters. */
    if (buf[2] < (size - 3))
        remain = buf[2];
    else
        remain = size - 3;

    while ((count - 3) < remain) {
        rd = read(fd, buf + count, remain - (count - 3));
        if (rd <= 0)
            return -1;
        count += rd;
    }

    UIM_END_FUNC();
    return count;
}

/* Function to read the Command complete event
 *
 * This will read the response for the command send to BT chip
 * and validate response against response opcode
 */
static int read_command_complete(int fd, unsigned short opcode)
{
    command_complete_t resp;

    UIM_START_FUNC();

    UIM_VER(" Command complete started");
    if (read_hci_event(fd, (unsigned char *)&resp, sizeof(resp)) < 0) {
        UIM_ERR(" Invalid response");
        return -1;
    }

    /* Response should be an event packet */
    if (resp.uart_prefix != HCI_EVENT_PKT) {
        UIM_ERR
            (" Error in response: not an event packet, but 0x%02x!",
             resp.uart_prefix);
        return -1;
    }

    /* Response should be a command complete event */
    if (resp.hci_hdr.evt != EVT_CMD_COMPLETE) {
        /* event must be event-complete */
        UIM_ERR
            (" Error in response: not a cmd-complete event,but 0x%02x!",
             resp.hci_hdr.evt);
        return -1;
    }

    if (resp.hci_hdr.plen < 4) {
        /* plen >= 4 for EVT_CMD_COMPLETE */
        UIM_ERR(" Error in response: plen is not >= 4, but 0x%02x!",
                resp.hci_hdr.plen);
        return -1;
    }

    if (resp.cmd_complete.opcode != (unsigned short)opcode) {
        UIM_ERR(" Error in response: opcode is 0x%04x, not 0x%04x!",
                resp.cmd_complete.opcode, opcode);
        return -1;
    }

    UIM_DBG(" Command complete done");
    return resp.status == 0 ? 0 : -1;
}




/*****************************************************************************
* Function to encode baudrate from int to char sequence
*****************************************************************************/
void
BRCM_encode_baud_rate(uint baud_rate, unsigned char *encoded_baud)
{
    if(baud_rate == 0 || encoded_baud == NULL) {
        fprintf(stderr, "Baudrate not supported!");
        return;
    }

    encoded_baud[3] = (unsigned char)(baud_rate >> 24);
    encoded_baud[2] = (unsigned char)(baud_rate >> 16);
    encoded_baud[1] = (unsigned char)(baud_rate >> 8);
    encoded_baud[0] = (unsigned char)(baud_rate & 0xFF);
}

void
BRCM_encode_bd_address( unsigned char  *bd_addrr)
{
    if(bd_addrr == NULL) {
        fprintf(stderr, "BD addr not supported!");
        return;
    }

    bd_addrr[0] = bd_addr.address[5];
    bd_addrr[1] = bd_addr.address[4];
    bd_addrr[2] = bd_addr.address[3];
    bd_addrr[3] = bd_addr.address[2];
    bd_addrr[4] = bd_addr.address[1];
    bd_addrr[5] = bd_addr.address[0];
}

typedef struct {
    int baud_rate;
    int termios_value;
} tBaudRates;

tBaudRates baud_rates[] = {
    { 115200, B115200 },
    { 230400, B230400 },
    { 460800, B460800 },
    { 500000, B500000 },
    { 576000, B576000 },
    { 921600, B921600 },
    { 1000000, B1000000 },
    { 1152000, B1152000 },
    { 1500000, B1500000 },
    { 2000000, B2000000 },
    { 2500000, B2500000 },
    { 3000000, B3000000 },
    { 4000000, B4000000 },
};


/*****************************************************************************
* Function to validate value of custom baudrate.
*****************************************************************************/
int
validate_baudrate(int baud_rate, int *value)
{
    unsigned int i;

    for (i = 0; i < (sizeof(baud_rates) / sizeof(tBaudRates)); i++) {
        if (baud_rates[i].baud_rate == baud_rate) {
            *value = baud_rates[i].termios_value;
            return(1);
        }
    }

    return(0);
}

/*****************************************************************************
* Function to set lpm param  value based on the config entry
*****************************************************************************/
static int proc_set_lpm_param()
{
    const char hci_writesleepmode_cmd[] = {0x01, 0x27, 0xFC, 0x0C, 0x00,0x00,0x001,\
                                          0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    char cmd[100];
    UIM_DBG("lpmenable %d",lpmenable);
    char *temp = hci_writesleepmode_cmd;
    strcat(hw_cfg_string, " lpm_param=");
    if(lpmenable) {
        memcpy((temp+4), &lpm_uim_param, LPM_CMD_PARAM_SIZE);
        UIM_DBG("%s lpm is enabled", __func__);
        }
    else
        memset((temp+4), 0, LPM_CMD_PARAM_SIZE);

    sprintf(cmd, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
              temp[0], temp[1], temp[2],temp[3], temp[4], temp[5],
              temp[6], temp[7], temp[8],temp[9], temp[10], temp[11],
              temp[12], temp[13], temp[14],temp[15]);
    strcat(hw_cfg_string , cmd);
    return 0;
}

/*****************************************************************************/
/* Function to set the UART custom baud rate.
 *
 * The UART baud rate has already been
 * set to default value 115200 before calling this function.
 * The baud rate is then changed to custom baud rate by this function
 *****************************************************************************/
static int proc_set_custom_baud_rate()
{
    unsigned char hci_update_baud_rate[] = { 0x01, 0x18, 0xFC, 0x06,0x00, \
                                               0x00,0x00,0x00,0x00,0x00 };
    int len;
    int termi_baudrate;

    UIM_START_FUNC();

    if (validate_baudrate(cust_baud_rate, &termi_baudrate)) {
         BRCM_encode_baud_rate(cust_baud_rate, &hci_update_baud_rate[6]);
    }
    else
        return -1;

    len = write(dev_fd, hci_update_baud_rate, 10);
    if (len < 0) {
        UIM_ERR(" failed to write baud rate\n");
        cleanup();
        return -1;
    }

    /* Read the response for the HCI set custom baud rate command */
    if (read_command_complete(dev_fd, HCI_RSP_OPCODE_SET_BAUDRATE) < 0) {
        UIM_ERR(" Invalid response to set custom baudrate");
        cleanup();
        return -1;
    }

    /* set custom baudrate */
    cfsetospeed(&termios, termi_baudrate);
    cfsetispeed(&termios, termi_baudrate);
    tcsetattr(dev_fd, TCSANOW, &termios);

    fcntl(dev_fd, F_SETFL,fcntl(dev_fd, F_GETFL) | O_NONBLOCK);
    UIM_DBG("baud rate set to %ld", cust_baud_rate);

    UIM_END_FUNC();
    return 0;
}


/*******************************************************************************
**
** Function         hw_strncmp
**
** Description      Used to compare two strings in caseless
**
** Returns          0: match, otherwise: not match
**
*******************************************************************************/
static int hw_strncmp (const char *p_str1, const char *p_str2, const int len)
{
    int i;

    if (!p_str1 || !p_str2)
        return (1);

    for (i = 0; i < len; i++)
    {
        if (toupper(p_str1[i]) != toupper(p_str2[i]))
            return (i+1);
    }

    return 0;
}


/*******************************************************************************
**
** Function         hw_config_findpatch
**
** Description      Search for a proper firmware patch file
**                  The selected firmware patch file name with full path
**                  will be stored in the input string parameter, i.e.
**                  p_chip_id_str, when returns.
**
** Returns          TRUE when found the target patch file, otherwise FALSE
**
*******************************************************************************/
static uint8_t hw_config_findpatch(char *p_chip_id_str)
{
    DIR *dirp;
    struct dirent *dp;
    int filenamelen;
    uint8_t retval = FALSE;

    if ((dirp = opendir(fw_patchfile_path)) != NULL)
    {
        UIM_DBG("Target name = [%s]", p_chip_id_str);
        /* Fetch next filename in patchfile directory */
        while ((dp = readdir(dirp)) != NULL)
        {
            /* Check if filename starts with chip-id name */
            if ((hw_strncmp(dp->d_name, p_chip_id_str, strlen(p_chip_id_str)) \
                ) == 0)
            {
                /* Check if it has .hcd extenstion */
                filenamelen = strlen(dp->d_name);
                if ((filenamelen >= FW_PATCHFILE_EXTENSION_LEN) &&
                    ((hw_strncmp(
                          &dp->d_name[filenamelen-FW_PATCHFILE_EXTENSION_LEN], \
                          FW_PATCHFILE_EXTENSION, \
                          FW_PATCHFILE_EXTENSION_LEN) \
                     ) == 0))
                {
                    UIM_DBG("Found patchfile: %s", dp->d_name);
                    /* Make sure length does not exceed maximum */
                    if (filenamelen >= FW_PATCH_FILENAME_MAXLEN)
                    {
                        UIM_ERR("Invalid patchfile name (too long) %s",
                            dp->d_name);
                        UIM_ERR("Max patchfile name length supported = %d",
                            FW_PATCH_FILENAME_MAXLEN-1);
                        break;
                    }

                    strcpy(p_chip_id_str, dp->d_name);
                    retval = TRUE;
                    break;
                }
            }
        }

        closedir(dirp);

        if (retval == FALSE)
        {
            /* Try again chip name without revision info */
            UIM_DBG("retval is FALSE");

            int len = strlen(p_chip_id_str);
            char *p = p_chip_id_str + len - 1;

            /* Scan backward and look for the first alphabet
               which is not M or m
            */
            while (len > 3) // BCM****
            {
                if ((isdigit(*p)==0) && (*p != 'M') && (*p != 'm'))
                    break;

                p--;
                len--;
            }

            if (len > 3)
            {
                *p = 0;
                retval = hw_config_findpatch(p_chip_id_str);
            }
        }
    }
    else
    {
        UIM_ERR("Could not open %s", fw_patchfile_path);
    }

    return (retval);
}


/*******************************************************************************
**
** Function         proc_read_local_name
**
** Description      Read local name of the chip. Find the correct patchram filename.
**                       Patchram filename is written to same input argument.
**
** Returns          0: match, otherwise: not match
**
*******************************************************************************/

static uint8_t proc_read_local_name(char* p_chip_id_str)
{
    unsigned char hci_read_localname[] = { 0x01, 0x14, 0x0C, 0x00 };
    unsigned char buff[READ_LOCALNAME_RESP_BUFF_SIZE];
    char *p_name, *p_tmp;
    int len, i;

    memset(buff, 0, sizeof(buff));

    len = write(dev_fd, hci_read_localname, 4);
    if (len < 0) {
        UIM_ERR(" request hci_read_localname failed");
        return FALSE;
    }

    if(read_hci_event(dev_fd, buff, FW_PATCH_FILENAME_MAXLEN) < 0) {
        UIM_ERR(" Invalid response for hci_read_localname");
        return FALSE;
    }

    p_name = p_tmp = buff + HCI_EVT_CMD_CMPL_LOCAL_NAME_STRING;
    UIM_DBG("readlocalname = %s", p_name);

    /* convert localname to upper case */
    for (i=0; (i < FW_PATCH_FILENAME_MAXLEN)||(*(p_name+i) != 0); i++)
        *(p_name+i) = toupper(*(p_name+i));

    /* Perform various checks on chip localname*/
    if ((p_name = strstr(p_name, "BCM")) != NULL)
    {
        strncpy(p_chip_id_str, p_name, \
                         FW_PATCH_FILENAME_MAXLEN-1);
                }
    else if((p_name = strstr(p_tmp,"4343")) != NULL)
    {
        snprintf(p_chip_id_str, FW_PATCH_FILENAME_MAXLEN-1, "BCM%s", p_name);
        strncpy(p_name, p_chip_id_str, FW_PATCH_FILENAME_MAXLEN-1);
    }
    else
    {
        strncpy(p_chip_id_str, "UNKNOWN", \
                    FW_PATCH_FILENAME_MAXLEN-1);
        return FALSE;
    }

    p_chip_id_str[FW_PATCH_FILENAME_MAXLEN-1] = 0;

    UIM_VER("Chipset %s", p_chip_id_str);

    return hw_config_findpatch(p_chip_id_str);
}


/*****************************************************************************
* Function to initialize UART
*****************************************************************************/
void proc_init_uart(int uart_fd, struct termios *termios)
{
    UIM_START_FUNC();
    tcflush(uart_fd, TCIOFLUSH);
    tcgetattr(uart_fd, termios);

    termios->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                | INLCR | IGNCR | ICRNL | IXON);
    termios->c_oflag &= ~OPOST;
    termios->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    termios->c_cflag &= ~(CSIZE | PARENB);
    termios->c_cflag |= CS8;

    termios->c_cflag |= CRTSCTS;
    tcsetattr(uart_fd, TCSANOW, termios);
    tcflush(uart_fd, TCIOFLUSH);
    tcsetattr(uart_fd, TCSANOW, termios);
    tcflush(uart_fd, TCIOFLUSH);
    tcflush(uart_fd, TCIOFLUSH);
    cfsetospeed(termios, B115200);
    cfsetispeed(termios, B115200);
    tcsetattr(uart_fd, TCSANOW, termios);

    UIM_END_FUNC();
    return;
}


/*****************************************************************************
 * This Function handles the Signals sent from the Kernel Init Manager.
 * After receiving the indication from rfkill subsystem, configure the
 * baud rate, flow control and Install the N_BRCM_HCI line discipline
 *
 * returns error code on failure, 0 on success.
 *****************************************************************************/
int st_uart_config(unsigned char install)
{
    int ldisc, proto;
    char local_chip_name[FW_PATCH_FILENAME_MAXLEN];
    int fw_fd;

    UIM_START_FUNC();

    if (install == V4L2_STATUS_ON) {
        UIM_DBG("install set to 1");

        if (dev_fd != -1) {
            UIM_ERR("opening %s, while already open", uart_port_name);
            return UIM_FAIL;
        }

        dev_fd = open((const char*) uart_port_name, O_RDWR);
        if (dev_fd < 0) {
            UIM_ERR(" Can't open %s", uart_port_name);
            UIM_ERR("read err (%s)", strerror(errno));
            return UIM_FAIL;
        }

        if (proc_bdaddr())
        {
            UIM_ERR("%s BD Address processing failed!!", __func__);
            return UIM_FAIL;
        }

        /* Perform UART initialization */
        proc_init_uart(dev_fd, &termios);

        /* Perform HCI reset */
        if(proc_hci_reset())
        {
            UIM_ERR("%s HCI RESET failed!!", __func__);
            return UIM_FAIL;
        }

        /* Set custom baud rate */
        if(proc_set_custom_baud_rate()) {
            UIM_ERR("Unable to set custom baud rate. Invalid baudrate!");
            return UIM_FAIL;
        }

        /* find patchram filename */
        if (strlen(fw_patchfile_name)> 0) {
            UIM_DBG("complete fw file name = %s", fw_patchfile_name);
        }
        else if(proc_read_local_name(local_chip_name) == TRUE) {
            UIM_DBG("complete fw file name = %s", local_chip_name);
            strcpy(fw_patchfile_name, local_chip_name);
        }
        else {
            UIM_ERR(" Can't get FW patchfile name");
            strcpy(fw_patchfile_name, "UNKNOWN");
        }

        UIM_VER("fw_patchfile_name = %s", fw_patchfile_name);
        /* write patchram filename to sysfs entry and pass to ldisc */
        fw_fd = open(FW_PATCHFILE_SYSFS_ENTRY, O_WRONLY);
        if (fw_fd < 0) {
            UIM_ERR(" Can't open %s", FW_PATCHFILE_SYSFS_ENTRY);
            return UIM_FAIL;
        }
        write(fw_fd, &fw_patchfile_name, strlen(fw_patchfile_name));
        UIM_DBG("writing to FW_PATCHFILE_SYSFS_ENTRY completed");
        close(fw_fd);

        /* After the UART speed has been changed, the IOCTL is called to set the
               *   line discipline to N_BRCM_HCI                                                     */
        ldisc = N_BRCM_HCI;
        proto = HCI_UART_H4;
        if (ioctl(dev_fd, TIOCSETD, &ldisc) < 0) {
            UIM_ERR(" Can't set line discipline N_BRCM_HCI=%d", N_BRCM_HCI);
            return UIM_FAIL;
        }

        if (ioctl(dev_fd, HCIUARTSETPROTO, proto) < 0) {
            UIM_ERR("Can't set hci protocol\n");
            return UIM_FAIL;
        }
        UIM_DBG("value of dev_fd = %d", dev_fd);
        UIM_DBG(" Installed N_BRCM_HCI=%d Line displine", N_BRCM_HCI);
    }

    UIM_END_FUNC();
    return 0;
}


/*****************************************************************************
* Function to process BD ADDRESS
*****************************************************************************/
int proc_bdaddr()
{
    bdstr_t bdstr;
    int fd =-1,res=0;

    /*Read BD Addr*/
    read_default_bdaddr(&bd_addr);

    /* pass bd_address to ldisc */
    bd2str(&bd_addr, &bdstr);
    UIM_DBG("%s, bdstr=%s", __func__, bdstr);

    fd = open(BDADDR_SYSFS_ENTRY, O_RDWR);
    if ((fd < 0)|| (write(fd, &bdstr, 18)<=0)){
        UIM_ERR("unable to open %s (%s)", BDADDR_SYSFS_ENTRY,strerror(errno));
        res =  UIM_FAIL;
    }
    if( fd )
        close(fd);
    return res;
}


/*****************************************************************************
* Function to perform HCI Reset
 *****************************************************************************/
int proc_hci_reset()
{
    const char hci_reset_cmd[] = {0x01, 0x03, 0x0C, 0x00};
    int len;

    UIM_START_FUNC();

    /* Perform HCI reset */
    len = write(dev_fd, hci_reset_cmd, 4);
    if (len < 0) {
        UIM_ERR(" failed to write HCI reset\n");
        cleanup();
        return -1;
    }

    /* Read the response for the HCI reset command */
    if (read_command_complete(dev_fd, HCI_RSP_OPCODE_HCI_RST) < 0) {
        UIM_ERR(" Invalid response to HCI Reset");
        cleanup();
        return -1;
    }

    UIM_END_FUNC();
    return 0;

}

/*****************************************************************************
* Function to convert the BD address from ascii to hex value
*****************************************************************************/
int str2bd(char *str, bdaddr_t *addr)
    {
        int32_t i = 0;
        for (i = 0; i < 6; i++)
        {
           addr->address[i] = (uint8_t)strtoul(str, &str, 16);
           str++;
        }
        return 0;
    }

/*****************************************************************************
* Function to read default bdaddress
*****************************************************************************/
#ifdef ANDROID
void read_default_bdaddr(bdaddr_t *local_addr)
{
    char val[256];
    uint8_t valid_bda = FALSE;

    const uint8_t null_bdaddr[BD_ADDR_LEN] = {0,0,0,0,0,0};

    /* Get local bdaddr storage path from property */
    if (property_get(PROPERTY_BT_BDADDR_PATH, val, NULL))
    {
        int addr_fd;

        UIM_VER("local bdaddr is stored in %s", val);

        if ((addr_fd = open(val, O_RDONLY)) != -1)
        {
            memset(val, 0, sizeof(val));
            read(addr_fd, val, FACTORY_BT_BDADDR_STORAGE_LEN);
            str2bd(val, local_addr);
            /* If this is not a reserved/special bda, then use it */
            if (memcmp(local_addr->address, null_bdaddr, BD_ADDR_LEN) != 0)
            {
                valid_bda = TRUE;
                UIM_DBG("Got Factory BDA %02X:%02X:%02X:%02X:%02X:%02X",
                    local_addr->address[0], local_addr->address[1],
                    local_addr->address[2], local_addr->address[3],
                    local_addr->address[4], local_addr->address[5]);
            }

            close(addr_fd);
        }
    }

    /* No factory BDADDR found. Look for previously generated random BDA */
    if ((!valid_bda) && \
        (property_get(PERSIST_BDADDR_PROPERTY, val, NULL)))
    {
        UIM_VER("local bdaddr is stored in %s", PERSIST_BDADDR_PROPERTY);
        str2bd(val, local_addr);
        valid_bda = TRUE;
        UIM_DBG("Got prior random BDA %02X:%02X:%02X:%02X:%02X:%02X",
            local_addr->address[0], local_addr->address[1], local_addr->address[2],
            local_addr->address[3], local_addr->address[4], local_addr->address[5]);
    }

    /* Generate new BDA if necessary */
    if (!valid_bda)
    {
        bdstr_t bdstr;
        /* Seed the random number generator */
        srand((unsigned int) (time(0)));

        /* No autogen BDA. Generate one now. */
        local_addr->address[0] = 0x22;
        local_addr->address[1] = 0x22;
        local_addr->address[2] = (uint8_t) ((rand() >> 8) & 0xFF);
        local_addr->address[3] = (uint8_t) ((rand() >> 8) & 0xFF);
        local_addr->address[4] = (uint8_t) ((rand() >> 8) & 0xFF);
        local_addr->address[5] = (uint8_t) ((rand() >> 8) & 0xFF);

        /* Convert to ascii, and store as a persistent property */
        bd2str(local_addr, &bdstr);

        UIM_ERR("No preset BDA. Generating BDA: %s for prop %s",
             (char*)bdstr, PERSIST_BDADDR_PROPERTY);

        if (property_set(PERSIST_BDADDR_PROPERTY, (char*)bdstr) < 0)
            UIM_ERR("Failed to set random BDA in prop %s",PERSIST_BDADDR_PROPERTY);
    }

    UIM_DBG("BDA to be set to %02X:%02X:%02X:%02X:%02X:%02X",
        local_addr->address[0], local_addr->address[1], local_addr->address[2],
        local_addr->address[3], local_addr->address[4], local_addr->address[5]);

    return;
}
#endif


/*****************************************************************************
* Main function
*****************************************************************************/
int main(void)
{
    int st_fd, err;
    struct stat file_stat;
    char kmodule_path[MAX_KMODULE_PATH_SIZE] = {0};
    struct pollfd p;
    unsigned char install;

    UIM_START_FUNC();
    err = 0;

    /* Read configuration parameters for hardware config */
    if (vnd_load_conf(VENDOR_LIB_CONF_FILE, &vendor_conf_table))
    {
        return UIM_FAIL;
    }

#if V4L2_SNOOP_ENABLE
    /* Read configuration parameters for hci snoop */
    if (vnd_load_conf(STACK_CONF_FILE, &stack_conf_table))
    {
        return UIM_FAIL;
    }
#endif

    //Set the LPM command parameters based on config entries
    proc_set_lpm_param();

    /* rfkill device's open/poll/read */
    UIM_DBG("Opening install before start of Polling");
    st_fd = open(INSTALL_SYSFS_ENTRY, O_RDONLY);
    if (st_fd < 0) {
        UIM_ERR("unable to open %s (%s)", INSTALL_SYSFS_ENTRY,
        strerror(errno));
        UIM_ERR("restarting uim");
        return UIM_FAIL;
    }

    /* pass parameters from bt_vendor.conf to ldisc for patchram download */
    if (pass_vendor_params())
    {
        UIM_ERR("unable to write vendor params to %s", LDISC_VENDOR_PARAMS);
        UIM_ERR("restarting uim");
        return UIM_FAIL;
    }

    UIM_DBG("first time polling");
    err = read(st_fd, &install, 1);
    if ((err > 0) && (install == V4L2_STATUS_ON)) {
        UIM_DBG("install already set");
        upio_set_bluetooth_power(1);
        // handle HCI snoop
        int fd_hcisnoop = -1;
        unsigned char snoop_enable = '0';
        if ((fd_hcisnoop = open(LDISC_SYSFS_SNOOP, O_RDONLY))< 0) {
            UIM_ERR("unable to open %s", LDISC_SYSFS_SNOOP);
        }
        else {
            read(fd_hcisnoop, &snoop_enable, 1);
            UIM_DBG("snoop_enable = %c", snoop_enable);
            close(fd_hcisnoop);
        }
        if ((hci_snoop_enable == 1) || (snoop_enable=='1'))
            v4l2_start_hci_snoop();
        if (st_uart_config(install) != 0)
        {
            UIM_ERR("st_uart_config failed");
            // cleanup as UIM failed to initialize uart
            err_cleanup(st_fd);
            return UIM_FAIL;
        }
    }

RE_POLL:

    UIM_DBG("begin polling");
    memset(&p, 0, sizeof(p));
    p.fd = st_fd;
    /* sysfs entries can only break poll for following events */
    p.events = POLLERR | POLLHUP;

RE_POLL_TILL_POLL_ERR:

    while (!exiting) {
        p.revents = 0;
        UIM_DBG("Polling to check POLLERR | POLLHUP on install fd");
        err = poll(&p, 1, -1);
        UIM_DBG("After Polling to check POLLERR | POLLHUP erro = %d", err);
        if (err < 0 && errno == EINTR){
            continue;
        }
        if (err) {
            UIM_DBG("Breaking out from RE_POLL_TILL_POLL_ERR while loop with err=%d", err);
            break;
        }
    }

    close(st_fd);
    st_fd = open(INSTALL_SYSFS_ENTRY, O_RDONLY);
    if (st_fd < 0) {
        UIM_ERR("re-opening %s failed: %s", INSTALL_SYSFS_ENTRY,
        strerror(errno));
        return UIM_FAIL;
    }

    if (!exiting)
    {
        err = read(st_fd, &install, 1);
        if (err <= 0) {
            UIM_ERR("reading %s failed: %s", INSTALL_SYSFS_ENTRY,
            strerror(errno));
            goto RE_POLL;
        }
        UIM_DBG("value of install = %c", install);
        UIM_DBG("value of dev_fd = %d",dev_fd);

        if ((install == V4L2_STATUS_ON) && (dev_fd == -1)) {
            UIM_DBG("set UART");
            upio_set_bluetooth_power(1);
            /* start hci snoop thread */
            // handle HCI snoop
            int fd_hcisnoop = -1;
            unsigned char snoop_enable = '0';
            if ((fd_hcisnoop = open(LDISC_SYSFS_SNOOP, O_RDONLY)) < 0){
                UIM_ERR("unable to open %s", LDISC_SYSFS_SNOOP);
            }
            else {
                read(fd_hcisnoop, &snoop_enable, 1);
                UIM_DBG("snoop_enable = %c", snoop_enable);
                close(fd_hcisnoop);
            }
            if ((hci_snoop_enable == 1) || (snoop_enable=='1'))
                v4l2_start_hci_snoop();
            if (st_uart_config(install)!=0)
            {
                UIM_ERR("st_uart_config failed");
                // cleanup as UIM failed to initialize uart
                err_cleanup(st_fd);
                return UIM_FAIL;
            }
            UIM_DBG("set UART done");
            goto RE_POLL_TILL_POLL_ERR;
        }
        else if (install == V4L2_STATUS_OFF) {
                // handle HCI snoop
                int fd_hcisnoop;
                unsigned char snoop_enable;
                fd_hcisnoop = open(LDISC_SYSFS_SNOOP, O_RDONLY);
                read(fd_hcisnoop, &snoop_enable, 1);
                UIM_DBG("snoop_enable = %c", snoop_enable);
                close(fd_hcisnoop);
                if(hci_snoop_enable == 1 || snoop_enable=='1'
                    || (v4l2_get_hci_snoop_status() == HCI_SNOOP_RUNNING))
                    v4l2_stop_hci_snoop();

                cleanup();
                UIM_VER("setting upio power to 0");
                upio_set_bluetooth_power(0);
                goto RE_POLL;
        }
        else if (install == V4L2_STATUS_ERR){
            /* In case of err (HCI timeout or Hardware error) in BT/FM/Ant,
               the application will write 0x02 to install sysfs entry. UIM upon
               detecting this value will restart UIM process. */
            cleanup();
            UIM_ERR("setting upio power to 0 for error recovery");
            upio_set_bluetooth_power(0);
            UIM_ERR("Closing shared transport fd - st_fd");
            close(st_fd);
            UIM_ERR("Restarting UIM due to error!");
            return UIM_FAIL;
        }
    }

    close(st_fd);
    UIM_END_FUNC();

    return 0;
}


