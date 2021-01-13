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
 * ble_sec.c
 * - Bluetooth security functions
 * ----------------------------------------------------------------------------
 * $Revision: 1.22 $
 * $Date: 2018/10/15 18:19:38 $
 * ------------------------------------------------------------------------- */

#include "app.h"

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_BondReqInd(ke_msg_id_t const msg_id,
 *                                     struct gapc_bond_req_ind const *param,
 *                                     ke_task_id_t const dest_id,
 *                                     ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Receive bond request and information exchange indication
 *                 from master, peripheral is storing the values that it
 *                 generates.IRK and LTK are generated and are sent to
 *                 master device
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_bond_req_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int GAPC_BondReqInd(ke_msg_id_t const msg_id,
                    struct gapc_bond_req_ind const *param,
                    ke_task_id_t const dest_id,
                    ke_task_id_t const src_id)
{
    /* Retrieve the index of environment structure representing peer device */
    signed int device_indx = Find_Connected_Device_Index(KE_IDX_GET(src_id));

    if (device_indx == INVALID_DEV_IDX)
    {
        return (KE_MSG_CONSUMED);
    }

    /* Prepare message to be sent to smpc */
    struct gapc_bond_cfm *cfm;
    uint8_t auth;
    uint8_t sec_req;

    /* Key and Random number has a different length
    * so the variable is used in two different loops
    * This variable is used in LTK_EXCH_REQ case */
    uint8_t counter;
    uint32_t pin_code;
    uint8_t *ptr;

    cfm = KE_MSG_ALLOC(GAPC_BOND_CFM, src_id,
                       KE_BUILD_ID(TASK_APP, device_indx), gapc_bond_cfm);

    /* Ensure that the current master is not already bonded
     * If the bonding information already exist, handle the request
     * differently */
    switch (param->request)
    {
        /* Prepare pairing information to be sent to the master device */
        case (GAPC_PAIRING_REQ):
        {
            /* If it's already bonded, remove old information from list to
             * create space for the new bond_info */
            if (VALID_BOND_INFO(ble_env[device_indx].bond_info.STATE))
            {
                ble_env[device_indx].bond_info.STATE = BOND_INFO_STATE_INVALID;
                BondList_Remove(&ble_env[device_indx].bond_info);
            }
#ifdef SECURE_CONNECTION
            if (param->data.auth_req & GAP_AUTH_REQ_SEC_CON_NO_BOND)
            {
                auth = GAP_AUTH_REQ_SEC_CON_BOND;
                sec_req = GAP_SEC1_SEC_CON_PAIR_ENC;
            }
            else
#endif    /* ifdef SECURE_CONNECTION */
            {
                auth = GAP_AUTH_REQ_NO_MITM_BOND;
                sec_req = GAP_NO_SEC;
            }

            /* If there is space in the BondList for a new entry,
             * accept request */
            if (BondList_Size() < APP_BONDLIST_SIZE)
            {
                cfm->request = GAPC_PAIRING_RSP;
                cfm->accept = 1;
                cfm->data.pairing_feat.auth = auth;
                cfm->data.pairing_feat.iocap =
                    GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
                cfm->data.pairing_feat.key_size  = KEY_LEN;
                cfm->data.pairing_feat.oob =
                    GAP_OOB_AUTH_DATA_NOT_PRESENT;
                cfm->data.pairing_feat.sec_req   = sec_req;
                cfm->data.pairing_feat.ikey_dist = GAP_KDIST_IDKEY   |
                                                   GAP_KDIST_SIGNKEY;

                cfm->data.pairing_feat.rkey_dist =
                    GAP_KDIST_ENCKEY  |
                    GAP_KDIST_IDKEY   |
                    GAP_KDIST_SIGNKEY;
            }
            else
            {
                cfm->accept = 0;
            }
        }
        break;

        /* Prepare LTK to be sent to the master device (legacy only) */
        case (GAPC_LTK_EXCH):
        {
            cfm->accept = true;
            cfm->request = GAPC_LTK_EXCH;

            /* Generate EDIV, LTK, and RAND for an encryption */
            cfm->data.ltk.ediv = (uint16_t)co_rand_word();

            for (counter = 0; counter < RAND_NB_LEN; counter++)
            {
                cfm->data.ltk.ltk.key[counter] = (uint8_t)co_rand_word();
                cfm->data.ltk.randnb.nb[counter] = (uint8_t)co_rand_word();
            }

            for (counter = RAND_NB_LEN; counter < KEY_LEN; counter++)
            {
                cfm->data.ltk.ltk.key[counter] = (uint8_t)co_rand_word();
            }

            /* Store the generated keys into the local list */
            memcpy(ble_env[device_indx].bond_info.LTK, cfm->data.ltk.ltk.key,
                   sizeof(cfm->data.ltk.ltk.key));
            memcpy(ble_env[device_indx].bond_info.RAND, cfm->data.ltk.randnb.nb,
                   sizeof(cfm->data.ltk.randnb.nb));
            memcpy(&ble_env[device_indx].bond_info.EDIV, &cfm->data.ltk.ediv,
                   sizeof(cfm->data.ltk.ediv));
        }
        break;

        /* Prepare TK to be sent to the master device */
        case (GAPC_TK_EXCH):
        {
            cfm->accept = true;
            cfm->request = GAPC_TK_EXCH;

            pin_code = (100000 + (co_rand_word() % 900000));

            /* Set the TK value */
            memset(cfm->data.tk.key, 0, KEY_LEN);

            cfm->data.tk.key[0] = (uint8_t)((pin_code & 0x000000FF) >>  0);
            cfm->data.tk.key[1] = (uint8_t)((pin_code & 0x0000FF00) >>  8);
            cfm->data.tk.key[2] = (uint8_t)((pin_code & 0x00FF0000) >> 16);
            cfm->data.tk.key[3] = (uint8_t)((pin_code & 0xFF000000) >> 24);
        }
        break;

        case (GAPC_IRK_EXCH):
        {
            cfm->accept = true;
            cfm->request = GAPC_IRK_EXCH;

            memcpy(cfm->data.irk.addr.addr.addr, bdaddr, BDADDR_LENGTH);
            cfm->data.irk.addr.addr_type = (bdaddr_type & 0x01);

            Device_Param_Read(PARAM_ID_IRK,
                              (uint8_t *)&cfm->data.irk.irk.key[0]);
        }
        break;

        case GAPC_CSRK_EXCH:
        {
            cfm->accept = true;
            cfm->request = GAPC_CSRK_EXCH;

            Device_Param_Read(PARAM_ID_CSRK,
                              (uint8_t *)&cfm->data.csrk.key[0]);
        }
        break;

        default:
        {
            /* Do nothing */
        } break;
    }

    /* Send the message */
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_BondInd(ke_msg_id_t const msg_id,
 *                                  struct gapc_bond_ind const *param,
 *                                  ke_task_id_t const dest_id,
 *                                  ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Receives bond procedure status and CSRK value
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_bond_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int GAPC_BondInd(ke_msg_id_t const msg_id,
                 struct gapc_bond_ind const *param,
                 ke_task_id_t const dest_id,
                 ke_task_id_t const src_id)
{
    /* Retrieve the index of environment structure representing peer device */
    signed int device_indx = Find_Connected_Device_Index(KE_IDX_GET(src_id));

    if (device_indx == INVALID_DEV_IDX)
    {
        return (KE_MSG_CONSUMED);
    }

    switch (param->info)
    {
        case (GAPC_PAIRING_SUCCEED):
        {
            ble_env[device_indx].bond_info.STATE = APP_BONDED;
            BondList_Add(&ble_env[device_indx].bond_info);
        }
        break;

        case (GAPC_IRK_EXCH):
        {
            memcpy(ble_env[device_indx].bond_info.IRK,
                   param->data.irk.irk.key,
                   sizeof(param->data.irk.irk.key));
            memcpy(ble_env[device_indx].bond_info.ADDR,
                   param->data.irk.addr.addr.addr,
                   sizeof(param->data.irk.addr.addr.addr));
            ble_env[device_indx].bond_info.ADDR_TYPE = param->data.irk.addr.addr_type;
        }
        break;

        case (GAPC_CSRK_EXCH):
        {
            memcpy(ble_env[device_indx].bond_info.CSRK, param->data.csrk.key,
                   sizeof(param->data.csrk.key));
        }
        break;

        case (GAPC_LTK_EXCH):
        {
            memcpy(ble_env[device_indx].bond_info.LTK, param->data.ltk.ltk.key,
                   sizeof(param->data.ltk.ltk.key));
            ble_env[device_indx].bond_info.EDIV = param->data.ltk.ediv;
            memcpy(ble_env[device_indx].bond_info.RAND,
                   param->data.ltk.randnb.nb,
                   sizeof(param->data.ltk.randnb.nb));
        }
        break;

        case (GAPC_REPEATED_ATTEMPT):
        case (GAPC_PAIRING_FAILED):
        default:
        {
        }
        break;
    }

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function       : int GAPC_EncryptInd(ke_msg_id_t const msg_id,
 *                                      struct gapc_encrypt_ind const *param,
 *                                      ke_task_id_t const dest_id,
 *                                      ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Receives this flag if encryption was successful
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_encrypt_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int GAPC_EncryptInd(ke_msg_id_t const msg_id,
                    struct gapc_encrypt_ind const *param,
                    ke_task_id_t const dest_id,
                    ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPC_EncryptReqInd(ke_msg_id_t const msg_id,
 *                                        struct gapc_encrypt_req_ind
 *                                        const *param,
 *                                        ke_task_id_t const dest_id,
 *                                        ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Receives encryption request from master device and sends
 *                 the encrpytion status after comparing the values from the
 *                 master device. Master device sends EDIV and RAND values
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapc_encrypt_req_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int GAPC_EncryptReqInd(ke_msg_id_t const msg_id,
                       struct gapc_encrypt_req_ind const *param,
                       ke_task_id_t const dest_id,
                       ke_task_id_t const src_id)
{
    /* Retrieve the index of environment structure representing peer device */
    signed int device_indx = Find_Connected_Device_Index(KE_IDX_GET(src_id));

    if (device_indx == INVALID_DEV_IDX)
    {
        return (KE_MSG_CONSUMED);
    }

    /* Prepare the GAPC_ENCRYPT_CFM message to be sent to SMPC */
    struct gapc_encrypt_cfm *cfm;
    cfm = KE_MSG_ALLOC(GAPC_ENCRYPT_CFM, src_id,
                       KE_BUILD_ID(TASK_APP, device_indx),
                       gapc_encrypt_cfm);

    /* If the devices are not bonded, do not accept the encryption request */
    cfm->found = false;

    /* If the devices are bonded, check the EDIV and RAND values sent
     * by the master */
    if (VALID_BOND_INFO(ble_env[device_indx].bond_info.STATE))
    {
        if ((param->ediv == ble_env[device_indx].bond_info.EDIV) &&
            (memcmp(param->rand_nb.nb, ble_env[device_indx].bond_info.RAND,
                    sizeof(struct rand_nb)) == 0))
        {
            cfm->found = true;
            cfm->key_size = KEY_LEN;
            memcpy(cfm->ltk.key, ble_env[device_indx].bond_info.LTK,
                   sizeof(struct gap_sec_key));
        }
    }

    /* Send the message */
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : int GAPM_AddrSolvedInd(ke_msg_idd_t const msg_id,
 *                                           struct gapm_addr_solved_ind
 *                                           const *param,
 *                                           ke_task_id_t const dest_id,
 *                                           ke_task_id_t const src_id)
 * ----------------------------------------------------------------------------
 * Description   : Handle address resolution message received from GAP manager
 * Inputs        : - msg_id     - Kernel message ID number
 *                 - param      - Message parameters in format of
 *                                struct gapm_addr_solved_ind
 *                 - dest_id    - Destination task ID number
 *                 - src_id     - Source task ID number
 * Outputs       : return value - Indicate if the message was consumed;
 *                                compare with KE_MSG_CONSUMED
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int GAPM_AddrSolvedInd(ke_msg_id_t const msg_id,
                       struct gapm_addr_solved_ind const *param,
                       ke_task_id_t const dest_id,
                       ke_task_id_t const src_id)
{
    uint8_t device_indx = KE_IDX_GET(dest_id);

    /* Load stored bond info */
    if (!BondList_FindByIRK(param->irk.key, &ble_env[device_indx].bond_info))
    {
        ble_env[device_indx].bond_info.STATE = BOND_INFO_STATE_INVALID;
        return (KE_MSG_CONSUMED);
    }

    Send_Connection_Confirmation(device_indx);
    BLE_SetServiceState(true, device_indx);

    return (KE_MSG_CONSUMED);
}

/* ----------------------------------------------------------------------------
 * Function      : bool BondList_Add(BondInfo_Type* bond_info)
 * ----------------------------------------------------------------------------
 * Description   : Add a new entry into the BondList. If NVR2 is full and there
 *                  are invalid entries in the list, the NVR2 is refreshed.
 * Inputs        : - bond_info      - pointer to a BondInfo_Type variable
 * Outputs       : - true if it was able to write the new entry into flash.
 *                 - false if list is full or if an error occurred while
 *                   writing to flash memory.
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
bool BondList_Add(BondInfo_Type *bond_info)
{
    /* If there is space in the BondList */
    if (BondList_Size() < APP_BONDLIST_SIZE)
    {
        BondInfo_Type list[APP_BONDLIST_SIZE];
        uint8_t *state;
        unsigned int data_size;
        unsigned int *data_to_write;
        uint32_t *start_addr = (uint32_t *)FLASH_NVR2_BASE;

        /* Check if NVR2 is full due to invalid entries. To do so, we only need
         * to check the state of last possible entry (SIZEOF_BONDLIST-1) */
        bool NVR2_full =
            *((uint8_t *)(FLASH_NVR2_BASE + (SIZEOF_BONDLIST - 1) *
                          sizeof(BondInfo_Type))) != BOND_INFO_STATE_EMPTY;

        /* If NVR2 is full, we will need to erase the whole sector and squeeze
         * the valid entries side-by-side. This will create space for the new
         * entry. */
        if (NVR2_full)
        {
            /* Copy valid entries to RAM */

            uint8_t copied = 0;

            for (uint8_t i = 0; i < SIZEOF_BONDLIST; i++)
            {
                state = (uint8_t *)(FLASH_NVR2_BASE + i *
                                    sizeof(BondInfo_Type));
                if (VALID_BOND_INFO(*state))
                {
                    /* Read bond_info entry from NVR2 */
                    memcpy(&list[copied],
                           (uint8_t *)(FLASH_NVR2_BASE +
                                       i * sizeof(BondInfo_Type)),
                           sizeof(BondInfo_Type));

                    /* Update STATE field with new index */
                    list[copied].STATE = copied + 1;
                    copied++;
                }
            }

            /* Add new bond_info to the end of the array */
            memcpy(&list[copied], bond_info, sizeof(BondInfo_Type));
            list[copied].STATE = copied + 1;
            copied++;

            /* Erase NVR2 */
            if (!BondList_RemoveAll())
            {
                return (false);
            }

            /* NVR2 sector was erased, will write everything to the base
             * address */
            start_addr = (uint32_t *)FLASH_NVR2_BASE;

            /* data_size is the current size + 1 (the new entry) */
            data_size = copied * sizeof(BondInfo_Type);

            /* The data_to_write is the list */
            data_to_write = (unsigned int *)list;
        }

        /* NVR2 has space available, we can write the new entry directly. */
        else
        {
            /* Search for the first available position in NVR2 */
            for (uint8_t i = 0; i < SIZEOF_BONDLIST; i++)
            {
                /* Read state of each bond_info entry in NVR2 */
                state = (uint8_t *)(FLASH_NVR2_BASE + i *
                                    sizeof(BondInfo_Type));
                if (*state == BOND_INFO_STATE_EMPTY)
                {
                    /* start_addr will be the first available index "i" */
                    start_addr = (uint32_t *)(FLASH_NVR2_BASE + i *
                                              sizeof(BondInfo_Type));

                    /* Update STATE field with new index */
                    bond_info->STATE = i + 1;
                    break;
                }
            }

            /* The data_to_write is directly the bond_info passed as argument */
            data_to_write = (unsigned int *)bond_info;

            /* We are going to write just 1 entry */
            data_size = sizeof(BondInfo_Type);
        }

        /* Write new data to flash */
        NVR2_WriteEnable(true);
        unsigned int result = Flash_WriteBuffer((unsigned int)start_addr,
                                                data_size / 4, data_to_write);
        NVR2_WriteEnable(false);

        return (result == FLASH_ERR_NONE);
    }

    return (false);
}

/* ----------------------------------------------------------------------------
 * Function      : bool BondList_FindByAddr(uint8_t* addr,
 *                                          const uint8_t addr_type,
 *                                          BondInfo_Type* result)
 * ----------------------------------------------------------------------------
 * Description   : Search for the bonding information for a peer device in
 *                 NVR2 with matching address and address type.
 * Inputs        : - addr      - address of the peer device to search
 *                 - addr_type - address type of the peer device to search
 * Outputs       : - result - pointer to a BondInfo_Type variable
 *                            that will hold the result of the search.
 *                 Return value:
 *                 - If found an entry with the specified address, return true
 *                   and the bonding information in the 'result' variable.
 *                 - If not found, return false.
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
bool BondList_FindByAddr(const uint8_t *addr,
                         const uint8_t addr_type,
                         BondInfo_Type *result)
{
    BondInfo_Type tmp;
    unsigned int i;

    for (i = 0; i < SIZEOF_BONDLIST; i++)
    {
        /* Read bond_info entry from NVR2 */
        memcpy(&tmp, (uint8_t *)(FLASH_NVR2_BASE + i * sizeof(BondInfo_Type)),
               sizeof(BondInfo_Type));

        /* If this bond_info entry is valid and the addresses match */
        if ((VALID_BOND_INFO(tmp.STATE)) &&
            (tmp.ADDR_TYPE == addr_type) &&
            (memcmp(tmp.ADDR, addr, sizeof(tmp.ADDR)) == 0))
        {
            /* Copy bond_info entry from NVR2 */
            tmp.ADDR_TYPE = (tmp.ADDR_TYPE & 0x01);
            memcpy(result, &tmp, sizeof(BondInfo_Type));
            break;
        }
    }

    return (i < SIZEOF_BONDLIST);
}

/* ----------------------------------------------------------------------------
 * Function      : bool BondList_FindByIRK(const uint8_t* irk,
 *                                         BondInfo_Type* result)
 * ----------------------------------------------------------------------------
 * Description   : Search for the bonding information for a peer device in
 *                 NVR2 with matching irk.
 * Inputs        : - irk            - irk to search
 *                 - result         - pointer to a BondInfo_Type variable
 *                                    that will hold the result of the search.
 * Outputs       : - If found an entry with the specified address, return true
 *                   and the bonding information in the 'result' variable.
 *                 - If not found, return false.
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
bool BondList_FindByIRK(const uint8_t *irk, BondInfo_Type *result)
{
    BondInfo_Type tmp;
    unsigned int i;

    for (i = 0; i < SIZEOF_BONDLIST; i++)
    {
        /* Read bond_info entry from NVR2 */
        memcpy(&tmp, (uint8_t *)(FLASH_NVR2_BASE + i * sizeof(BondInfo_Type)),
               sizeof(BondInfo_Type));

        /* If this bond_info entry is valid and the addresses match */
        if (VALID_BOND_INFO(tmp.STATE) &&
            (memcmp(tmp.IRK, irk, sizeof(tmp.IRK)) == 0))
        {
            /* Copy bond_info entry from NVR2 */
            memcpy(result, &tmp, sizeof(BondInfo_Type));
            break;
        }
    }

    return (i < SIZEOF_BONDLIST);
}

/* ----------------------------------------------------------------------------
 * Function      : bool BondList_Remove(BondInfo_Type *target)
 * ----------------------------------------------------------------------------
 * Description   : Remove the bonding information associated to a peer device
 *                 with address 'addr'.
 * Inputs        : - target           - peer device bond info to be removed
 * Outputs       : - Return true if it was able to find and remove an entry
 *                   with the specified address.
 *                 - Return false otherwise.
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
bool BondList_Remove(BondInfo_Type *target)
{
    BondInfo_Type tmp;
    bool result = false;

    if (BondList_FindByAddr(target->ADDR, target->ADDR_TYPE, &tmp))
    {
        unsigned int data = BOND_INFO_STATE_INVALID;

        /* NVR2 address where the bonding information will be invalidated */
        uint32_t *addr = (uint32_t *)(FLASH_NVR2_BASE +
                                      (tmp.STATE - 1) * sizeof(BondInfo_Type));

        /* Invalidate the state for the old bonding information stored in
         * NVR2 by clearing the first two words. */
        NVR2_WriteEnable(true);
        result = (Flash_WriteWordPair((unsigned int)addr, data, data) ==
                  FLASH_ERR_NONE);
        NVR2_WriteEnable(false);
    }

    return (result);
}

/* ----------------------------------------------------------------------------
 * Function      : void BondList_RemoveAll(void)
 * ----------------------------------------------------------------------------
 * Description   : Clear BondList in flash by erasing NVR2 sector.
 * Inputs        : None
 * Outputs       : true if NVR2 has been erased successfully, false otherwise
 * Assumptions   : Reset the board in case it cannot enable write on NVR2
 * ------------------------------------------------------------------------- */
bool BondList_RemoveAll(void)
{
    uint32_t temp = 0xFFFFFFFF;
    unsigned int result = FLASH_ERR_NONE;

    /* Checks if NVR2 is already erased such that it will not write again */
    for(uint32_t *ptr = (uint32_t*)FLASH_NVR2_BASE; ptr < (uint32_t*)FLASH_NVR2_TOP; ptr++)
    {
        if (*ptr != temp)
        {
            /* Erase NVR2 sector */
            NVR2_WriteEnable(true);
            result = Flash_EraseSector(FLASH_NVR2_BASE);
            NVR2_WriteEnable(false);
            break;
        }
    }

    return (result == FLASH_ERR_NONE);
}

/* ----------------------------------------------------------------------------
 * Function      : uint8_t BondList_Size(void)
 * ----------------------------------------------------------------------------
 * Description   : Calculates the number of entries in the BondList
 *                 information
 * Inputs        : None
 * Outputs       : Number of entries in the BondList
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint8_t BondList_Size(void)
{
    uint8_t size = 0;
    uint8_t *state;

    for (unsigned int i = 0; i < SIZEOF_BONDLIST; i++)
    {
        /* Read state of each bond_info entry from NVR2 */
        state = (uint8_t *)(FLASH_NVR2_BASE + i * sizeof(BondInfo_Type));
        if (VALID_BOND_INFO(*state))
        {
            size++;
        }
    }

    return (size);
}

/* ----------------------------------------------------------------------------
 * Function      : uint8_t BondList_GetIRKs(const struct gap_sec_key *irkList)
 * ----------------------------------------------------------------------------
 * Description   : Returns the number of IRK keys along with the keys
 * Inputs        : irkList      - Pointer to an empty array that will hold the
 *                                result
 * Outputs       : - The number of IRK keys present in the BondList
 *                 - irkList array filled with all IRK keys in the BondList
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint8_t BondList_GetIRKs(struct gap_sec_key *irkList)
{
    uint8_t numKeys = 0;
    BondInfo_Type tmp;

    for (unsigned int i = 0; i < SIZEOF_BONDLIST; i++)
    {
        /* Read bond_info entry from NVR2 */
        memcpy(&tmp, (uint8_t *)(FLASH_NVR2_BASE + i * sizeof(BondInfo_Type)),
               sizeof(BondInfo_Type));

        /* If this bond_info entry is valid and the addresses match */
        if (VALID_BOND_INFO(tmp.STATE))
        {
            memcpy(&irkList[numKeys], tmp.IRK, sizeof(tmp.IRK));
            numKeys++;
        }
    }

    return (numKeys);
}

/* ----------------------------------------------------------------------------
 * Function      : unsigned NVR2_WriteEnable(bool enable)
 * ----------------------------------------------------------------------------
 * Description   : Enable/Disable writing to NVR2
 * Inputs        : - enable    - Enable writing when true, disable otherwise
 * Outputs       : None
 * Assumptions   : Function should cause a watchdog reset in case of error
 * ------------------------------------------------------------------------- */
void NVR2_WriteEnable(bool enable)
{
    /* Lock or unlock NVR2 region */
    FLASH->NVR_CTRL = NVR2_WRITE_ENABLE;
    FLASH->NVR_WRITE_UNLOCK = enable ? FLASH_NVR_KEY : 0;

    /* Check the lock/unlock operation result */
    bool NVR2_unlocked = (FLASH->IF_STATUS & 0x3FF) == 0x20;

    /* Error checking: this application needs to write in flash. If an error
     * occurred while locking/unlocking NVR2, wait for a watchdog reset */
    if (NVR2_unlocked != enable)
    {
        /* Disable all interrupts and clear any pending interrupts */
        Sys_NVIC_DisableAllInt();
        Sys_NVIC_ClearAllPendingInt();
        while (1)
        {
            /* Wait for Watchdog Reset to Occur */
            asm ("nop");
        }
    }
}
