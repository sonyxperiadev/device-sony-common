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
 */


/************************************************************************************
*
*  Filename:      brcm_uim.h
*
*  Description:   User Interface Module header file.
*
***********************************************************************************/


#ifndef UIM_H
#define UIM_H

#define TRUE 1
#define FALSE 0

/*HCI Command and Event information*/
#define HCI_HDR_OPCODE          0xff36
#define WRITE_BD_ADDR_OPCODE    0xFC06
#define RESP_PREFIX             0x04
#define MAX_TRY                 10

/* HCI Packet types */
#define HCI_COMMAND_PKT         0x01
#define HCI_EVENT_PKT           0x04


/* HCI command macros */
#define HCI_EVENT_HDR_SIZE              2
#define HCI_COMMAND_HDR_SIZE            3
#define UIM_WRITE_BD_ADDR_CP_SIZE       6


/* HCI event macros */
#define EVT_CMD_COMPLETE_SIZE   3
#define EVT_CMD_STATUS_SIZE     4
#define EVT_CMD_COMPLETE        0x0E
#define EVT_CMD_STATUS          0x0F

/* Set proto for shared LDISC */
#define HCIUARTSETPROTO     _IOW('U', 200, int)
#define HCI_UART_H4     0
#define HCI_UART_BCSP   1
#define HCI_UART_3WIRE  2
#define HCI_UART_H4DS   3
#define HCI_UART_LL     4

#define LPM_SLEEP_MODE 1
#define LPM_IDLE_THRESHOLD 1
#define LPM_HC_IDLE_THRESHOLD 1
#define LPM_BT_WAKE_POLARITY 1
#define LPM_HOST_WAKE_POLARITY 1
#define LPM_ALLOW_HOST_SLEEP_DURING_SCO 1
#define LPM_COMBINE_SLEEP_MODE_AND_LPM 1
#define LPM_ENABLE_UART_TXD_TRI_STATE 0
#define LPM_PULSED_HOST_WAKE 0
#define LPM_CMD_PARAM_SIZE 12

/* low power mode parameters */
typedef struct
{
    uint8_t sleep_mode;                     /* 0(disable),1(UART),9(H5) */
    uint8_t host_stack_idle_threshold;      /* Unit scale 300ms/25ms */
    uint8_t host_controller_idle_threshold; /* Unit scale 300ms/25ms */
    uint8_t bt_wake_polarity;               /* 0=Active Low, 1= Active High */
    uint8_t host_wake_polarity;             /* 0=Active Low, 1= Active High */
    uint8_t allow_host_sleep_during_sco;
    uint8_t combine_sleep_mode_and_lpm;
    uint8_t enable_uart_txd_tri_state;      /* UART_TXD Tri-State */
    uint8_t sleep_guard_time;               /* sleep guard time in 12.5ms */
    uint8_t wakeup_guard_time;              /* wakeup guard time in 12.5ms */
    uint8_t txd_config;                     /* TXD is high in sleep state */
    uint8_t pulsed_host_wake;               /* pulsed host wake if mode = 1 */
} btuim_lpm_param_t;

/* HCI command header*/
typedef struct {
    uint16_t        opcode;         /* OCF & OGF */
    uint8_t         plen;
} __attribute__ ((packed))      hci_command_hdr;

/* HCI event header*/
typedef struct {
    uint8_t         evt;
    uint8_t         plen;
} __attribute__ ((packed))      hci_event_hdr;

/* HCI command complete event*/
typedef struct {
    uint8_t         ncmd;
    uint16_t        opcode;
} __attribute__ ((packed)) evt_cmd_complete;

/* HCI event status*/
typedef struct {
    uint8_t         status;
    uint8_t         ncmd;
    uint16_t        opcode;
} __attribute__ ((packed)) evt_cmd_status;

/* HCI Event structure to set the cusrom baud rate*/
typedef struct {
    uint8_t uart_prefix;
    hci_event_hdr hci_hdr;
    evt_cmd_complete cmd_complete;
    uint8_t status;
    uint8_t data[16];
} __attribute__ ((packed)) command_complete_t;

typedef struct {
    unsigned char address[6];
} __attribute__((packed)) bdaddr_t;

/* HCI Command structure to set the uim BD address*/
typedef struct {
    uint8_t uart_prefix;
    hci_command_hdr hci_hdr;
    bdaddr_t addr;
} __attribute__ ((packed)) uim_bdaddr_change_cmd;

/* Sys_fs entry. The Line discipline driver sets this to 1 when bluedroid open BT protocol driver */
/* Note: This entry is used in bt_hci_bdroid.c (Android source). Also present in
 *  brcm_sh_ldisc.c (v4l2_drivers) and board specific file (android kernel source) */
#define INSTALL_SYSFS_ENTRY "/sys/bus/platform/drivers/bcm_ldisc/bcmbt_ldisc.93/install"
#define LDISC_VENDOR_PARAMS   "/sys/bus/platform/drivers/bcm_ldisc/bcmbt_ldisc.93/vendor_params"

#define BDADDR_SYSFS_ENTRY  "/sys/bus/platform/drivers/bcm_ldisc/bcmbt_ldisc.93/bdaddr"
#define FW_PATCHFILE_SYSFS_ENTRY  "/sys/bus/platform/drivers/bcm_ldisc/bcmbt_ldisc.93/fw_patchfile"
#define LDISC_SYSFS_SNOOP     "/sys/bus/platform/drivers/bcm_ldisc/bcmbt_ldisc.93/snoop_enable"

/* install sysfs entry values */
#define V4L2_STATUS_ERR '2'  // error occured in BT application (HCI command timeout or HW error)
#define V4L2_STATUS_ON  '1'  // Atleast one procol driver is registered
#define V4L2_STATUS_OFF '0'  // No procol drivers registered

/* File used for hardware config. Read udev_name, baudrate and module path from this file */
#define VENDOR_LIB_CONF_FILE "/etc/bluetooth/bt_vendor.conf"


/* HCI response opcodes */
#define HCI_RSP_OPCODE_HCI_RST        0x0c03
#define HCI_RSP_OPCODE_HCI_LPM_DIS        0xfc27
#define HCI_RSP_OPCODE_SET_BAUDRATE   0xfc18
#define HCI_RSP_OPCODE_HCI_UART_CLOCK_SET        0xfc45

#define MAX_KMODULE_PATH_SIZE 100
#define UART_PORT_NAME_SIZE 20

#define N_BRCM_HCI 26


/* Functions to insert and remove the kernel modules from the system*/
extern int init_module(void *, unsigned int, const char *);
extern int delete_module(const char *, unsigned int);

/* Function declarations */
void proc_init_uart(int uart_fd, struct termios *termios);
int st_uart_config(unsigned char install);
int proc_hci_reset();
extern int upio_set_bluetooth_power(int on);
int proc_hci_uartclockset();
void read_default_bdaddr();
int proc_bdaddr();

#endif /* UIM_H */
