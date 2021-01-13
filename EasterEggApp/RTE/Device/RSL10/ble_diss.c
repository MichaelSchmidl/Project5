/* ----------------------------------------------------------------------------
 * Copyright (c) 2018 Semiconductor Components Industries, LLC (d/b/a
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
 * ble_diss.c
 * - Bluetooth device information service server functions
 * ------------------------------------------------------------------------- */
#include <rsl10.h>
#include <ble_diss.h>
#include <ble_gap.h>
#include <msg_handler.h>
#include <prf_utils.h>
#include <diss.h>

/* Global variable definition */
static DISS_Env_t diss_env;

/* ----------------------------------------------------------------------------
 * Function      : void DISS_Initialize(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize device information server service environment and
 *                 configure message handlers
 * Inputs        : features - The enabled features for the device information
 *                  service. Values define as enum diss_features in diss_task.h
 * Outputs       : 0 if the initialization was successful, < 0 otherwise
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
/**
 * @brief	Initialize device information server service environment and configure
 *  		message handlers
 *
 * Assumptions   : None
 *
 * @param[in] features The enabled features for the device information
 *                  service. Values define as enum diss_features in \ref diss_task.h
 * @return	0 if the initialization was successful, < 0 otherwise
 */
int DISS_Initialize(uint16_t features, const struct DISS_DeviceInfo_t *deviceInfo)
{
    memset(&diss_env, 0, sizeof(DISS_Env_t));

    diss_env.features = features;
    diss_env.deviceInfo = deviceInfo;


    /*
     * Add DISS message handlers
     */
    /* General  */
    MsgHandler_Add(TASK_ID_DISS,            DISS_MsgHandler);
    MsgHandler_Add(GAPM_CMP_EVT,            DISS_MsgHandler);
    MsgHandler_Add(GAPM_PROFILE_ADDED_IND,  DISS_MsgHandler);
    MsgHandler_Add(DISS_SET_VALUE_RSP,      DISS_MsgHandler);
    MsgHandler_Add(DISS_VALUE_REQ_IND,      DISS_DeviceInfoValueReqInd);

    return 0;
}

/* ----------------------------------------------------------------------------
 * Function      : void DeviceInfo_ValueReqInd(ke_msg_id_t const msg_id,
 *                                     struct diss_value_req_ind
 *                                     const *param,
 *                                     ke_task_id_t const dest_id,
 *                                     ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle the indication when peer device requests a value
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct diss_value_req_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
/**
 * @brief Handle the indication when peer device requests a value
 *
 * Assumptions   : None
 *
 * @param[in] msgid Kernel message ID number
 * @param[in] param Message parameters in format of struct diss_value_req_ind
 * @param[in] dest_id Destination task ID number
 * @param[in] src_id Source task ID number
 */
void DISS_DeviceInfoValueReqInd( ke_msg_id_t const msgid, void const * param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id )
{
    struct diss_value_req_ind const *p = param;

    const struct DISS_DeviceInfo_t *deviceInfo = diss_env.deviceInfo;
   /* Initialize length */
    uint8_t len = 0;

    /* Pointer to the data */
    uint8_t *data;

    /* Check requested value */
    switch( p->value )
    {
        case DIS_MANUFACTURER_NAME_CHAR:
        {
            /* Set information */
            len = deviceInfo->MANUFACTURER_NAME.len;
            data = deviceInfo->MANUFACTURER_NAME.data;
        }
        break;

        case DIS_MODEL_NB_STR_CHAR:
        {
            /* Set information */
            len = deviceInfo->MODEL_NB_STR.len;
            data = deviceInfo->MODEL_NB_STR.data;
        }
        break;

        case DIS_SYSTEM_ID_CHAR:
        {
            /* Set information */
            len = deviceInfo->SYSTEM_ID.len;
            data = deviceInfo->SYSTEM_ID.data;
        }
        break;

        case DIS_PNP_ID_CHAR:
        {
            /* Set information */
            len = deviceInfo->PNP.len;
            data = deviceInfo->PNP.data;
        }
        break;

        case DIS_SERIAL_NB_STR_CHAR:
        {
            /* Set information */
            len = deviceInfo->SERIAL_NB_STR.len;
            data = deviceInfo->SERIAL_NB_STR.data;
        }
        break;

        case DIS_HARD_REV_STR_CHAR:
        {
            /* Set information */
            len = deviceInfo->HARD_REV_STR.len;
            data = deviceInfo->HARD_REV_STR.data;
        }
        break;

        case DIS_FIRM_REV_STR_CHAR:
        {
            /* Set information */
            len = deviceInfo->FIRM_REV_STR.len;
            data = deviceInfo->FIRM_REV_STR.data;
        }
        break;

        case DIS_SW_REV_STR_CHAR:
        {
            /* Set information */
            len = deviceInfo->SW_REV_STR.len;
            data = deviceInfo->SW_REV_STR.data;
        }
        break;

        case DIS_IEEE_CHAR:
        {
            /* Set information */
            len = deviceInfo->IEEE.len;
            data = deviceInfo->IEEE.data;
        }
        break;

        default:
        break;
    }


    /* Allocate confirmation to send the value */
    struct diss_value_cfm *cfm_value = KE_MSG_ALLOC_DYN(DISS_VALUE_CFM, src_id,
            dest_id, diss_value_cfm, len);

    /* Set parameters */
    cfm_value->value = p->value;
    cfm_value->length = len;
    if (len)
    {
        /* Copy data */
        memcpy(&cfm_value->data[0], data, len);
    }

    /* Send message */
    ke_msg_send(cfm_value);
}

/* ----------------------------------------------------------------------------
 * Function      : void DISS_ProfileTaskAddCmd(void)
 * ----------------------------------------------------------------------------
 * Description   : Send request to add the device information service in kernel and DB
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
/**
 * @brief Send request to add the device information service in kernel and DB
 *
 * <a href="https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.device_information.xml">Device information Service</a>
 */
void DISS_ProfileTaskAddCmd(void)
{
    struct diss_db_cfg db_cfg;

    /* Configure DISS database parameters  */
    db_cfg.features = diss_env.features;

    /* Add DISS profile task to the stack. A GAPM_PROFILE_ADDED_IND event
     * is generated when finished. */
    GAPM_ProfileTaskAddCmd(PERM(SVC_AUTH, ENABLE), TASK_ID_DISS, TASK_APP,
                           0, (uint8_t*)&db_cfg, sizeof(struct diss_db_cfg));
}

/* ----------------------------------------------------------------------------
 * Function      : void DISS_GetEnv(void)
 * ----------------------------------------------------------------------------
 * Description   : Return a reference to the internal environment structure.
 * Inputs        : None
 * Outputs       : A constant pointer to DISS_Env_t
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
const DISS_Env_t* DISS_EnvGet(void)
{
    return &diss_env;
}

/* ----------------------------------------------------------------------------
 * Function      : bool DISS_IsAdded(void)
 * ----------------------------------------------------------------------------
 * Description   : Return a boolean indication is the DISS server has been
 *                  successfully added to the services database.
 * Inputs        : None
 * Outputs       : Return value: true if the service as added, false if not
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
/**
 * @brief Return a boolean indication is the DISS server has been successfully
 *          added to the services database.
 *
 * Assumptions   : None
 *
 * @return true if the service as added, false if not
 */
bool DISS_IsAdded(void)
{
    return diss_env.serviceAdded;
}

/* ----------------------------------------------------------------------------
 * Function      : void DISS_MsgHandler(ke_msg_id_t const msg_id,
 *                                      void const *param,
 *                                      ke_task_id_t const dest_id,
 *                                      ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : 	Handle all events related to the device information service
 * 					server
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameter
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
/**
 * @brief Handle all events related to the device information service server
 *
 * Assumptions   : None
 *
 * @param[in] msg_id Kernel message ID number
 * @param[in] param Message parameter
 * @param[in] dest_id Destination task ID number
 * @param[in] src_id Source task ID number
 */
void DISS_MsgHandler(ke_msg_id_t const msg_id, void const *param,
                     ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    switch(msg_id)
    {
        case GAPM_CMP_EVT:	// GAP initialized, add the Device Information Service
        {					// will receive a GAPM_PROFILE_ADDED_IND event as response
            if(((struct gapm_cmp_evt*)param)->operation == GAPM_SET_DEV_CONFIG)
            {
                DISS_ProfileTaskAddCmd();
            }
        }
        break;

        case GAPM_PROFILE_ADDED_IND:
        {
            struct gapm_profile_added_ind *ind = (struct gapm_profile_added_ind*) param;
            if( ind->prf_task_id == TASK_ID_DISS )
            {
                diss_env.serviceAdded = true;
            }
        }
        break;

        case DISS_SET_VALUE_RSP:
        {
        }
        break;

        default:
        break;
    }
}
