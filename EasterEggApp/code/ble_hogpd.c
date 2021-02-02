/* ----------------------------------------------------------------------------
 * Copyright (c) 2017 Semiconductor Components Industries, LLC (d/b/a
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
 *  This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * ble_hogpd.c
 * - HID over GATT device role service functions
 * ----------------------------------------------------------------------------
 * $Revision: 1.10 $
 * $Date: 2018/02/27 15:42:17 $
 * ------------------------------------------------------------------------- */

#include "app.h"

/* Global variable definition */
struct hogpd_support_env_tag hogpd_support_env;

/* HID Report Map characteristic value */
/* Keyboard report descriptor (using format for Boot interface descriptor) */
static const uint8_t hid_report_map[] =
{
    0x05, 0x01,    /* Usage Page (Generic Desktop) */
    0x09, 0x02,    /* Usage (Mouse) */
    0xA1, 0x01,    /* Collection (Application) */
    0x85, 0x01,    /* Report Id (1) */
    0x09, 0x01,    /*   Usage (Pointer) */
    0xA1, 0x00,    /*   Collection (Physical) */
    0x05, 0x09,    /*     Usage Page (Buttons) */
    0x19, 0x01,    /*     Usage Minimum (01) - Button 1 */
    0x29, 0x03,    /*     Usage Maximum (03) - Button 3 */
    0x15, 0x00,    /*     Logical Minimum (0) */
    0x25, 0x01,    /*     Logical Maximum (1) */
    0x75, 0x01,    /*     Report Size (1) */
    0x95, 0x03,    /*     Report Count (3) */
    0x81, 0x02,    /*     Input (Data, Variable, Absolute) - Button states */
    0x75, 0x05,    /*     Report Size (5) */
    0x95, 0x01,    /*     Report Count (1) */
    0x81, 0x01,    /*     Input (Constant) - Padding or Reserved bits */
    0x05, 0x01,    /*     Usage Page (Generic Desktop) */
    0x09, 0x30,    /*     Usage (X) */
    0x09, 0x31,    /*     Usage (Y) */
    0x09, 0x38,    /*     Usage (Wheel) */
    0x15, 0x81,    /*     Logical Minimum (-127) */
    0x25, 0x7F,    /*     Logical Maximum (127) */
    0x75, 0x08,    /*     Report Size (8) */
    0x95, 0x03,    /*     Report Count (3) */
    0x81, 0x06,    /*     Input (Data, Variable, Relative) - X & Y coordinate */
    0xC0,      /*   End Collection */
    0xC0,      /* End Collection */

    0x05, 0x01,    /* Usage Pg (Generic Desktop) */
    0x09, 0x06,    /* Usage (Keyboard) */
    0xA1, 0x01,    /* Collection: (Application) */
    0x85, 0x02,    /* Report ID (2) */
                   /* */
    0x05, 0x07,    /* Usage Pg (Key Codes) */
    0x19, 0xE0,    /* Usage Min (224) */
    0x29, 0xE7,    /* Usage Max (231) */
    0x15, 0x00,    /* Log Min (0) */
    0x25, 0x01,    /* Log Max (1) */
                   /* */
                   /* Modifier byte */
    0x75, 0x01,    /* Report Size (1) */
    0x95, 0x08,    /* Report Count (8) */
    0x81, 0x02,    /* Input: (Data, Variable, Absolute) */
                   /* */
                   /* Reserved byte */
    0x95, 0x01,    /* Report Count (1) */
    0x75, 0x08,    /* Report Size (8) */
    0x81, 0x01,    /* Input: (Constant) */
                   /* */
                   /* LED report */
    0x95, 0x05,    /* Report Count (5) */
    0x75, 0x01,    /* Report Size (1) */
    0x05, 0x08,    /* Usage Pg (LEDs) */
    0x19, 0x01,    /* Usage Min (1) */
    0x29, 0x05,    /* Usage Max (5) */
    0x91, 0x02,    /* Output: (Data, Variable, Absolute) */
                   /* */
                   /* LED report padding */
    0x95, 0x01,    /* Report Count (1) */
    0x75, 0x03,    /* Report Size (3) */
    0x91, 0x01,    /* Output: (Constant) */
                   /* */
                   /* Key arrays (6 bytes) */
    0x95, 0x06,    /* Report Count (6) */
    0x75, 0x08,    /* Report Size (8) */
    0x15, 0x00,    /* Log Min (0) */
    0x25, 0x65,    /* Log Max (101) */
    0x05, 0x07,    /* Usage Pg (Key Codes) */
    0x19, 0x00,    /* Usage Min (0) */
    0x29, 0x65,    /* Usage Max (101) */
    0x81, 0x00,    /* Input: (Data, Array) */
                   /* */
    0xC0,         /* End Collection */
                  /* */
    0x05, 0x0C,    /* Usage Pg (Consumer Devices) */
    0x09, 0x01,    /* Usage (Consumer Control) */
    0xA1, 0x01,    /* Collection (Application) */
    0x85, 0x03,    /* Report Id (3) */
    0x09, 0x02,    /*   Usage (Numeric Key Pad) */
    0xA1, 0x02,    /*   Collection (Logical) */
    0x05, 0x09,    /*     Usage Pg (Button) */
    0x19, 0x01,    /*     Usage Min (Button 1) */
    0x29, 0x0A,    /*     Usage Max (Button 10) */
    0x15, 0x01,    /*     Logical Min (1) */
    0x25, 0x0A,    /*     Logical Max (10) */
    0x75, 0x04,    /*     Report Size (4) */
    0x95, 0x01,    /*     Report Count (1) */
    0x81, 0x00,    /*     Input (Data, Ary, Abs) */
    0xC0,       /*   End Collection */
    0x05, 0x0C,    /*   Usage Pg (Consumer Devices) */
    0x09, 0x86,    /*   Usage (Channel) */
    0x15, 0xFF,    /*   Logical Min (-1) */
    0x25, 0x01,    /*   Logical Max (1) */
    0x75, 0x02,    /*   Report Size (2) */
    0x95, 0x01,    /*   Report Count (1) */
    0x81, 0x46,    /*   Input (Data, Var, Rel, Null) */
    0x09, 0xE9,    /*   Usage (Volume Up) */
    0x09, 0xEA,    /*   Usage (Volume Down) */
    0x15, 0x00,    /*   Logical Min (0) */
    0x75, 0x01,    /*   Report Size (1) */
    0x95, 0x02,    /*   Report Count (2) */
    0x81, 0x02,    /*   Input (Data, Var, Abs) */
    0x09, 0xE2,    /*   Usage (Mute) */
    0x09, 0x30,    /*   Usage (Power) */
    0x09, 0x32,    /*   Usage (Sleep) */
    0x09, 0xB4,    /*   Usage (Rewind) */
    0x09, 0xB0,    /*   Usage (Play) */
    0x09, 0xB1,    /*   Usage (Pause) */
    0x09, 0xB3,    /*   Usage (Fast Forward) */
    0x09, 0xB5,    /*   Usage (Scan Next) */
    0x09, 0xB6,    /*   Usage (Scan Prev) */
    0x09, 0xB7,    /*   Usage (Stop) */
    0x15, 0x01,    /*   Logical Min (1) */
    0x25, 0x0A,    /*   Logical Max (10) */
    0x75, 0x04,    /*   Report Size (4) */
    0x95, 0x01,    /*   Report Count (1) */
    0x81, 0x00,    /*   Input (Data, Ary, Abs) */
    /* Sel usages 1 */
    0x0a, 0x00, 0x02,              /*   USAGE (Generating GUI Application
                                    * Buttons) */
    0xa1, 0x02,                    /*   COLLECTION (Logical) */
    0x15, 0x01,                    /*     LOGICAL_MINIMUM (1) */
    0x25, 0x08,                    /*     LOGICAL_MAXIMUM (8) */
    0x75, 0x04,                    /*     REPORT_SIZE (4) */
    0x95, 0x01,                    /*     REPORT_COUNT (1) */
    0x0a, 0x1F, 0x02,              /*     USAGE (AC Find)   - index 1 */
    0x0a, 0x23, 0x02,              /*     USAGE (AC Home)   - index 2 */
    0x0a, 0x1C, 0x02,              /*     USAGE (AC Cut)    - index 3 */
    0x0a, 0x1B, 0x02,              /*     USAGE (AC Copy)   - index 4 */
    0x0a, 0x1D, 0x02,              /*     USAGE (AC Paste)  - index 5 */
    0x0a, 0x22, 0x02,              /*     USAGE (AC Go To)  - index 6 */
    0x0a, 0x21, 0x02,              /*     USAGE (AC Search) - index 7 */
    0x0a, 0x27, 0x02,              /*     USAGE (AC Refresh)- index 8 */
    0x81, 0x00,                    /*     INPUT (Data,Ary,Abs) - index 0 means
                                    * none of above is selected */
    0xc0,    /*     END_COLLECTION */
    0xC0       /* End Collection */
};

/* HID report map length */
static const uint8_t hid_report_map_len = sizeof(hid_report_map);

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_Env_Initialize(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize HID over GATT device role environment
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_Env_Initialize(void)
{
    memset(&hogpd_support_env, 0, sizeof(hogpd_support_env));
}

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_ServiceAdd(void)
 * ----------------------------------------------------------------------------
 * Description   : Send request to add HID over GATT service in kernel and DB
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_ServiceAdd(void)
{
    struct hogpd_db_cfg *db_cfg;

    /* Allocate the HOGPD_CREATE_DB_REQ */
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(
        GAPM_PROFILE_TASK_ADD_CMD,
        TASK_GAPM,
        TASK_APP,
        gapm_profile_task_add_cmd,
        sizeof(struct
               hogpd_db_cfg));

    /* Fill message */
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, ENABLE);
    req->prf_task_id = TASK_ID_HOGPD;
    req->app_task = TASK_APP;
    req->start_hdl = HOGPD_SERV_START_HANDLE;

    /* Set Parameters */
    db_cfg = (struct hogpd_db_cfg *)req->param;
    db_cfg->hids_nb = HOGPD_NUM_OF_HID_DEV;
    db_cfg->cfg[0].svc_features = HOGPD_CFG_KEYBOARD | HOGPD_CFG_PROTO_MODE;
    db_cfg->cfg[0].report_nb = HOGPD_NUM_OF_HID_REP;
    db_cfg->cfg[0].report_char_cfg[0] = HOGPD_CFG_REPORT_IN;
    db_cfg->cfg[0].report_id[0] = HOGPD_NUM_OF_HID_RID;
    db_cfg->cfg[0].hid_info.bCountryCode = HOGPD_NUM_OF_HID_CC;
    db_cfg->cfg[0].hid_info.bcdHID = HOGPD_NUM_OF_HID_VER;
    db_cfg->cfg[0].hid_info.flags = HIDS_REMOTE_WAKE_CAPABLE |
                                    HIDS_NORM_CONNECTABLE;
    db_cfg->cfg[0].ext_ref.inc_svc_hdl = HOGPD_NUM_OF_HID_EXT;
    db_cfg->cfg[0].ext_ref.rep_ref_uuid = HOGPD_NUM_OF_HID_EXT;

    /* Send message */
    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_EnableReq(const uint8_t coidx)
 * ----------------------------------------------------------------------------
 * Description   : This API message can be used after the connection with a
 *                 peer device has been established in order to restore BOND
 *                 data of a known device
 * Inputs        : - conidx - connection index
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_EnableReq(const uint8_t conidx)
{
    struct hogpd_enable_req *req;
    struct hogpd_env_tag *hogpd_env;
    hogpd_env = PRF_ENV_GET(HOGPD, hogpd);
    hogpd_support_env.state = HID_ENABLED;

    /* Allocate the message to enable hogpd service server */
    req = KE_MSG_ALLOC(HOGPD_ENABLE_REQ,
                       prf_src_task_get(&(hogpd_env->prf_env),
                                        conidx),
                       TASK_APP, hogpd_enable_req);
    req->conidx = conidx;
    req->ntf_cfg[0] = HOGPD_CFG_KEYBOARD | HOGPD_CFG_REPORT_NTF_EN;

    /* Send the message */
    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : int Hogpd_EnableRsp(ke_msg_id_t const msg_id,
 *                                     struct hogpd_enable_rsp
 *                                     const *param,
 *                                     ke_task_id_t const dest_id,
 *                                     ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle HID service enable response from HOGPD task
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct hogpd_enable_rsp
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int Hogpd_EnableRsp(ke_msg_id_t const msg_id,
                    struct hogpd_enable_rsp const *param,
                    ke_task_id_t const dest_id, ke_task_id_t const
                    src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        hogpd_support_env.enable = true;
//        Restart_Keystroke_Env();
    }

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : int Hogpd_NtfCfgInd(ke_msg_id_t const msg_id,
 *                                          struct hogpd_ntf_cfg_ind
 *                                          const *param,
 *                                          ke_task_id_t const dest_id,
 *                                          ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle indication of configuration changes for HID
 *                 notification
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct hogpd_ntf_cfg_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int Hogpd_NtfCfgInd(ke_msg_id_t const msg_id,
                    struct hogpd_ntf_cfg_ind const *param,
                    ke_task_id_t const dest_id, ke_task_id_t const
                    src_id)
{
    /* Retrieve the index of environment structure representing peer device */
    signed int device_indx = Find_Connected_Device_Index(KE_IDX_GET(src_id));

    if (device_indx == INVALID_DEV_IDX)
    {
        return (KE_MSG_CONSUMED);
    }

    if (ble_env[device_indx].conidx == param->conidx)
    {
        if ((param->ntf_cfg[param->conidx] & HOGPD_CFG_REPORT_NTF_EN) != 0)
        {
            /* The device is ready to send reports to the peer device */
            hogpd_support_env.state = HID_READY;
        }
        else
        {
            /* Come back to the Enabled state */
            if (hogpd_support_env.state == HID_READY)
            {
                hogpd_support_env.state = HID_ENABLED;
            }
        }
    }
    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : int Hogpd_ProtoModeReqInd(ke_msg_id_t const msg_id,
 *                                          struct hogpd_proto_mode_req_ind
 *                                          const *param,
 *                                          ke_task_id_t const dest_id,
 *                                          ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle indication of Protocol Mode Characteristic Value for
 * HID
 *                 has been written on Device
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct hogpd_proto_mode_req_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int Hogpd_ProtoModeReqInd(ke_msg_id_t const msgid,
                          struct hogpd_proto_mode_req_ind const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id)
{
    /* Retrieve the index of environment structure representing peer device */
    signed int device_indx = Find_Connected_Device_Index(KE_IDX_GET(src_id));

    if (device_indx == INVALID_DEV_IDX)
    {
        return (KE_MSG_CONSUMED);
    }

    uint8_t status;
    if ((param->conidx == ble_env[device_indx].conidx) &&
        (param->operation == HOGPD_OP_PROT_UPDATE))
    {
        status = GAP_ERR_NO_ERROR;
    }
    else
    {
        status = ATT_ERR_APP_ERROR;
    }
    Hogpd_ProtoModeCfm(ble_env[device_indx].conidx, status, param->proto_mode);

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_ProtoModeCfm(const uint8_t conidx)
 * ----------------------------------------------------------------------------
 * Description   : This API message can be used to confirm or not modification
 *                 of the protocol mode
 * Inputs        : - conidx - connection index
 *                 - status - status of the protocol mode indication
 *                 - proto_mode - protocol mode
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_ProtoModeCfm(const uint8_t conidx,
                        const uint8_t status,
                        const uint8_t proto_mode)
{
    /* reply for Hogpd_ProtoModeReqInd */
    struct hogpd_proto_mode_cfm *req;
    struct hogpd_env_tag *hogpd_env;
    hogpd_env = PRF_ENV_GET(HOGPD, hogpd);

    req = KE_MSG_ALLOC(HOGPD_PROTO_MODE_CFM,
                       prf_src_task_get(&(hogpd_env->prf_env), conidx),
                       TASK_APP,
                       hogpd_proto_mode_cfm);

    req->conidx = conidx;
    req->status = status;
    req->hid_idx = conidx;
    req->proto_mode = proto_mode;

    /* Send the message */
    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : int Hogpd_CtnlPtInd(ke_msg_id_t const msg_id,
 *                                          struct hogpd_ctnl_pt_ind
 *                                          const *param,
 *                                          ke_task_id_t const dest_id,
 *                                          ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle indication of HID Control Point Characteristic for
 *                 HID has been written on Device
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct hogpd_proto_mode_req_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int Hogpd_CtnlPtInd(ke_msg_id_t const msgid,
                    struct hogpd_ctnl_pt_ind const *param,
                    ke_task_id_t const dest_id,
                    ke_task_id_t const src_id)
{
    /* Retrieve the index of environment structure representing peer device */
    signed int device_indx = Find_Connected_Device_Index(KE_IDX_GET(src_id));

    if (device_indx == INVALID_DEV_IDX)
    {
        return (KE_MSG_CONSUMED);
    }

    if (param->conidx == ble_env[device_indx].conidx)
    {
        Hogpd_CtnlPtReportCfm(param->conidx, GAP_ERR_NO_ERROR,
                              HOGPD_OP_REPORT_WRITE);
    }

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_ReportUpdReq(const uint8_t coidx,
 *                                         const uint8_t *input_rep,
 *                                         const uint16_t rep_len)
 * ----------------------------------------------------------------------------
 * Description   : This API message can be used to update the value of the
 *                 Report Characteristic stored in the database and to
 *                 notify the Host about this new value if sending of
 *                 notifications has been enabled for it.
 * Inputs        : - conidx - connection index
 *                 - key - keystroke to send to host
 *                 - mod_id - modifier to send to host
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_ReportUpdReq(const uint8_t conidx,
                        const uint8_t key,
                        const uint8_t mod_id)
{
    /* reply for Hogpd_ProtoModeReqInd */
    struct hogpd_report_upd_req *req;
    struct hogpd_env_tag *hogpd_env;
    hogpd_env = PRF_ENV_GET(HOGPD, hogpd);

    req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ,
                           prf_src_task_get(&(hogpd_env->prf_env),
                                            conidx),
                           TASK_APP,
                           hogpd_report_upd_req,
                           HOGPD_REPORT_LEN);

    req->conidx = conidx;
    req->report.hid_idx = 0;
    req->report.type = HOGPD_REPORT;
    req->report.length = HOGPD_REPORT_LEN;
    req->report.idx = 0;

    req->report.value[0] = mod_id;
    req->report.value[1] = 0x00;
    req->report.value[2] = key;
    req->report.value[3] = 0x00;
    req->report.value[4] = 0x00;
    req->report.value[5] = 0x00;
    req->report.value[6] = 0x00;
    req->report.value[7] = 0x00;

    /* Send the message */
    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : int Hogpd_ReportUpdRsp(ke_msg_id_t const msg_id,
 *                                     struct hogpd_report_upd_rsp
 *                                     const *param,
 *                                     ke_task_id_t const dest_id,
 *                                     ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : This message is sent to the application after reception
 *                 of the HOGPD_REPORT_UPD_REQ to inform it if a notification
 *                 has been sent to the Host or if an error has been raised
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct hogpd_report_upd_rsp
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int Hogpd_ReportUpdRsp(ke_msg_id_t const msgid,
                       struct hogpd_report_upd_rsp const *param,
                       ke_task_id_t const dest_id,
                       ke_task_id_t const src_id)
{
    /* Retrieve the index of environment structure representing peer device */
    signed int device_indx = Find_Connected_Device_Index(KE_IDX_GET(src_id));

    if (device_indx == INVALID_DEV_IDX)
    {
        return (KE_MSG_CONSUMED);
    }

    if (param->status != GAP_ERR_NO_ERROR)
    {
        hogpd_support_env.err_cnt++;
    }

    if (app_env.key_state == KEY_REL)
    {
        Hogpd_ReportUpdReq(ble_env[device_indx].conidx, HOGPD_KEY_NONE,
                           HOGPD_KEY_NONE);
        EGG_doneWithSendingKeyStroke();
    }

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : int Hogpd_ReportReqInd(ke_msg_id_t const msg_id,
 *                                     struct hogpd_report_req_ind
 *                                     const *param,
 *                                     ke_task_id_t const dest_id,
 *                                     ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : This message is sent to the application after the peer
 *                 Host has written the value of one of the Report
 *                 Characteristics or if peer device request information
 *                 of a report value.
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct hogpd_report_upd_rsp
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int Hogpd_ReportReqInd(ke_msg_id_t const msgid,
                       struct hogpd_report_req_ind const *param,
                       ke_task_id_t const dest_id,
                       ke_task_id_t const src_id)
{
    if ((param->operation == HOGPD_OP_REPORT_READ) && (param->report.type ==
                                                       HOGPD_REPORT_MAP))
    {
        Hogpd_ReportMapCfm(param->report.hid_idx, src_id);
    }
    else
    {
        if (param->report.type == HOGPD_BOOT_KEYBOARD_INPUT_REPORT)
        {
            Hogpd_BootKbInputReportCfm(param->conidx,
                                       param->report.type,
                                       param->report.idx);
        }
        else    /* HOGPD_REPORT */
        {
            Hogpd_DefaultReportCfm(param->conidx,
                                   param->report.type,
                                   param->report.idx,
                                   param->report.hid_idx,
                                   param->report.length);
        }
    }

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_CtnlPtReportCfm(const uint8_t conidx,
 *                                            const uint8_t status,
 *                                            const uint8_t op)
 * ----------------------------------------------------------------------------
 * Description   : This message is sent by the application to confirm
 *                 read or modification of a report
 * Inputs        : - conidx - connection index
 *                 - status - status of the report characteristics request
 *                 - op - operation to perform on the report
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_CtnlPtReportCfm(const uint8_t conidx,
                           const uint8_t status,
                           const uint8_t op)
{
    /* reply for Hogpd_ReportReqInd */
    struct hogpd_report_cfm *req;
    struct hogpd_env_tag *hogpd_env;
    hogpd_env = PRF_ENV_GET(HOGPD, hogpd);

    req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                           prf_src_task_get(&(hogpd_env->prf_env),
                                            conidx),
                           TASK_APP,
                           hogpd_report_cfm,
                           0);

    req->conidx = conidx;
    req->operation = op;
    req->status = status;
    req->report.hid_idx = conidx;
    req->report.type = -1;
    req->report.length = 0;
    req->report.idx = 0;

    /* Send the message */
    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_ReportMapCfm(const uint8_t hid_idx)
 * ----------------------------------------------------------------------------
 * Description   : This message is sent by the application to confirm
 *                 read or modification of a report
 * Inputs        : - hid_idx - HID index
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_ReportMapCfm(const uint8_t hid_idx, ke_task_id_t const src_id)
{
    /* Retrieve the index of environment structure representing peer device */
    signed int device_indx = Find_Connected_Device_Index(KE_IDX_GET(src_id));

    if (device_indx == INVALID_DEV_IDX)
    {
        return;
    }

    /* reply for Hogpd_ReportReqInd */
    struct hogpd_report_cfm *req;
    struct hogpd_env_tag *hogpd_env;
    hogpd_env = PRF_ENV_GET(HOGPD, hogpd);

    req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                           prf_src_task_get(&(hogpd_env->prf_env),
                                            ble_env[device_indx].conidx),
                           TASK_APP,
                           hogpd_report_cfm,
                           hid_report_map_len);

    req->conidx = ble_env[device_indx].conidx;
    req->operation = HOGPD_OP_REPORT_READ;
    req->status = GAP_ERR_NO_ERROR;
    req->report.hid_idx = hid_idx;
    req->report.type = HOGPD_REPORT_MAP;
    req->report.length = hid_report_map_len;
    req->report.idx = 0;

    memcpy(&req->report.value[0], &hid_report_map[0], hid_report_map_len);

    /* Send the message */
    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_BootKbInputReportCfm(const uint8_t conidx,
 *                                                 const uint8_t status,
 *                                                 const uint8_t op)
 * ----------------------------------------------------------------------------
 * Description   : This message is sent by the application to confirm
 *                 read or modification of a report
 * Inputs        : - conidx - connection index
 *                 - type - type of the report
 *                 - idx - index of the report
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_BootKbInputReportCfm(const uint8_t conidx,
                                const uint8_t type,
                                const uint8_t idx)
{
    /* reply for Hogpd_ReportReqInd */
    struct hogpd_report_cfm *req;
    struct hogpd_env_tag *hogpd_env;
    hogpd_env = PRF_ENV_GET(HOGPD, hogpd);

    req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                           prf_src_task_get(&(hogpd_env->prf_env),
                                            conidx),
                           TASK_APP,
                           hogpd_report_cfm,
                           0);

    req->conidx = conidx;
    req->operation = HOGPD_OP_REPORT_READ;
    req->status = GAP_ERR_NO_ERROR;
    req->report.hid_idx = conidx;
    req->report.type = type;
    req->report.length = 0;
    req->report.idx = idx;

    /* Send the message */
    ke_msg_send(req);
}

/* ----------------------------------------------------------------------------
 * Function      : void Hogpd_DefaultReportCfm(const uint8_t conidx,
 *                                             const uint8_t type,
 *                                             const uint8_t idx,
 *                                             const uint8_t hid_idx,
 *                                             const uint8_t len)
 * ----------------------------------------------------------------------------
 * Description   : This message is sent by the application to confirm
 *                 read or modification of a report
 * Inputs        : - conidx - connection index
 *                 - type - type of the report
 *                 - idx - index of the report,
 *                 - hid_idx - HID index
 *                 - len - length of the report
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Hogpd_DefaultReportCfm(const uint8_t conidx,
                            const uint8_t type,
                            const uint8_t idx,
                            const uint8_t hid_idx,
                            const uint8_t len)
{
    /* reply for Hogpd_ReportReqInd */
    struct hogpd_report_cfm *req;
    struct hogpd_env_tag *hogpd_env;
    hogpd_env = PRF_ENV_GET(HOGPD, hogpd);

    req = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                           prf_src_task_get(&(hogpd_env->prf_env),
                                            conidx),
                           TASK_APP,
                           hogpd_report_cfm,
                           HOGPD_REPORT_LEN);

    req->conidx = conidx;
    req->operation = HOGPD_OP_REPORT_READ;
    req->status = GAP_ERR_NO_ERROR;
    req->report.hid_idx = conidx;
    req->report.type = type;
    req->report.length = HOGPD_REPORT_LEN;
    req->report.idx = idx;

    req->report.value[0] = hid_idx;
    req->report.value[1] = type;
    req->report.value[2] = len;
    req->report.value[3] = idx;
    req->report.value[4] = 0x00;
    req->report.value[5] = 0x00;
    req->report.value[6] = 0x00;
    req->report.value[7] = 0x00;

    /* Send the message */
    ke_msg_send(req);
}
