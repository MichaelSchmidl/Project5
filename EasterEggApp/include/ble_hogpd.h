/* ----------------------------------------------------------------------------
 * Copyright (c) 2015-2017 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 * This module is derived in part from example code provided by RivieraWaves
 * and as such the underlying code is the property of RivieraWaves [a member
 * of the CEVA, Inc. group of companies], together with additional code which
 * is the property of ON Semiconductor. The code (in whole or any part) may not
 * be redistributed in any form without prior written permission from
 * ON Semiconductor.
 *
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * ble_hogpd.h
 * - HID over GATT device role service functions
 * ----------------------------------------------------------------------------
 * $Revision: 1.10 $
 * $Date: 2018/02/27 15:46:06 $
 * ------------------------------------------------------------------------- */

#ifndef BLE_HOGPD_H
#define BLE_HOGPD_H

/* ----------------------------------------------------------------------------
 * If building with a C++ compiler, make all of the definitions in this header
 * have a C binding.
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

/* ----------------------------------------------------------------------------
 * Include files
 * --------------------------------------------------------------------------*/

#include <hogpd.h>

#define HOGPD_SERV_START_HANDLE 0x00
#define HOGPD_NUM_OF_HID_DEV    0x01
#define HOGPD_NUM_OF_HID_REP    0x01
#define HOGPD_NUM_OF_HID_RID    0x02
#define HOGPD_NUM_OF_HID_CC     0x00
#define HOGPD_NUM_OF_HID_VER    0x0111
#define HOGPD_NUM_OF_HID_EXT    0x0000
#define HOGPD_REPORT_LEN        8
#define HOGPD_KEY_NONE          0x00

/* ----------------------------------------------------------------------------
 * Defines
 * --------------------------------------------------------------------------*/

/* List of message handlers that are used by the HID over GATT profile for
 * device role application manager */
#define HOGPD_MESSAGE_HANDLER_LIST                                           \
    DEFINE_MESSAGE_HANDLER(HOGPD_ENABLE_RSP, Hogpd_EnableRsp),               \
    DEFINE_MESSAGE_HANDLER(HOGPD_NTF_CFG_IND, Hogpd_NtfCfgInd),              \
    DEFINE_MESSAGE_HANDLER(HOGPD_PROTO_MODE_REQ_IND, Hogpd_ProtoModeReqInd), \
    DEFINE_MESSAGE_HANDLER(HOGPD_CTNL_PT_IND, Hogpd_CtnlPtInd),              \
    DEFINE_MESSAGE_HANDLER(HOGPD_REPORT_UPD_RSP, Hogpd_ReportUpdRsp),        \
    DEFINE_MESSAGE_HANDLER(HOGPD_REPORT_REQ_IND, Hogpd_ReportReqInd)

/* ----------------------------------------------------------------------------
 * Global variables and types
 * --------------------------------------------------------------------------*/
struct hogpd_support_env_tag
{
    /* HID Control Point Characteristic value */
    uint8_t hid_ctnl_pt;

    /* The flag that indicates that service has been enabled */
    bool enable;

    /* Counting the number of errors */
    uint32_t err_cnt;

    /* HID state */
    uint8_t state;
};

/* States of the HID */
enum app_hid_states
{
    /* Module is disabled (Service not added in DB) */
    HID_DISABLED,

    /* Module is idle (Service added but profile not enabled) */
    HID_IDLE,

    /* Module is enabled (Device is connected and the profile is enabled) */
    HID_ENABLED,

    /* The application can send reports */
    HID_READY,

    /* Waiting for a report */
    HID_WAIT_REP,

    HID_STATE_MAX,
};

/* Support for the application manager and the application environment */
extern struct hogpd_support_env_tag hogpd_support_env;

/* ----------------------------------------------------------------------------
 * Function prototype definitions
 * --------------------------------------------------------------------------*/

/* Bluetooth HID over GATT profile for device role (HOGPD) support functions */
extern void Hogpd_Env_Initialize(void);

extern void Hogpd_ServiceAdd(void);

extern void Hogpd_EnableReq(const uint8_t conidx);

extern int Hogpd_EnableRsp(ke_msg_id_t const msgid,
                           struct hogpd_enable_rsp const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id);

extern int Hogpd_NtfCfgInd(ke_msg_id_t const msgid,
                           struct hogpd_ntf_cfg_ind const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id);

extern int Hogpd_ProtoModeReqInd(ke_msg_id_t const msgid,
                                 struct hogpd_proto_mode_req_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id);

extern void Hogpd_ProtoModeCfm(const uint8_t conidx,
                               const uint8_t status,
                               const uint8_t proto_mode);

extern int Hogpd_CtnlPtInd(ke_msg_id_t const msgid,
                           struct hogpd_ctnl_pt_ind const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id);

extern void Hogpd_ReportUpdReq(const uint8_t coidx,
                               const uint8_t key,
                               const uint8_t mod_id);

extern int Hogpd_ReportUpdRsp(ke_msg_id_t const msgid,
                              struct hogpd_report_upd_rsp const *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id);

extern int Hogpd_ReportReqInd(ke_msg_id_t const msgid,
                              struct hogpd_report_req_ind const *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id);

extern void Hogpd_CtnlPtReportCfm(const uint8_t conidx,
                                  const uint8_t status,
                                  const uint8_t op);

extern void Hogpd_ReportMapCfm(const uint8_t hid_idx,
                               ke_task_id_t const src_id);

extern void Hogpd_BootKbInputReportCfm(const uint8_t conidx,
                                       const uint8_t type,
                                       const uint8_t idx);

extern void Hogpd_DefaultReportCfm(const uint8_t conidx,
                                   const uint8_t type,
                                   const uint8_t idx,
                                   const uint8_t hid_idx,
                                   const uint8_t len);

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* BLE_HOGPD_H */
