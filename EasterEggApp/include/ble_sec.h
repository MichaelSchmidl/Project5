/* ----------------------------------------------------------------------------
 * Copyright (c) 2015-2017 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *
 * This is Reusable Code.
 *
 * ----------------------------------------------------------------------------
 * ble_sec.h
 * - Bluetooth security header
 * ----------------------------------------------------------------------------
 * $Revision: 1.11 $
 * $Date: 2018/05/10 20:54:41 $
 * ------------------------------------------------------------------------- */

#ifndef BLE_SEC_H
#define BLE_SEC_H

/* ----------------------------------------------------------------------------
 * If building with a C++ compiler, make all of the definitions in this header
 * have a C binding.
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif    /* ifdef __cplusplus */

/* ----------------------------------------------------------------------------
 * Defines
 * --------------------------------------------------------------------------*/

/* To disable secure connection, comment out the following definition */
#define SECURE_CONNECTION

/* Bonding status sent to the master device */
#define APP_BONDED                      0x01
#define APP_NOT_BONDED                  0XFF

/* Check if a bondlist entry is valid */
#define VALID_BOND_INFO(state)          ((state > BOND_INFO_STATE_INVALID) && \
                                         (state <= SIZEOF_BONDLIST))
/* Size of the application BondList. Maximum APP_BONDLIST_SIZE allowed is
 * 28 entries (defined in SIZEOF_BONDLIST in rsl10_map_nvr.h) */
#define APP_BONDLIST_SIZE               20

#define APP_IRK                         { 0x01, 0x23, 0x45, 0x68, 0x78, 0x9a, \
                                          0xbc, 0xde, 0x01, 0x23, 0x45, 0x68, \
                                          0x78, 0x9a, 0xbc, 0xde }
#define APP_CSRK                        { 0x01, 0x23, 0x45, 0x68, 0x78, 0x9a, \
                                          0xbc, 0xde, 0x01, 0x23, 0x45, 0x68, \
                                          0x78, 0x9a, 0xbc, 0xde }

#define APP_PRIVATE_KEY                 { 0xEC, 0x89, 0x3C, 0x11, 0xBB, 0x2E, \
                                          0xEB, 0x5C, 0x80, 0x88, 0x63, 0x57, \
                                          0xCC, 0xE2, 0x05, 0x17, 0x20, 0x75, \
                                          0x5A, 0x26, 0x3E, 0x8D, 0xCF, 0x26, \
                                          0x63, 0x1D, 0x26, 0x0B, 0xCE, 0x4D, \
                                          0x9E, 0x07 }

#define APP_PUBLIC_KEY_X                { 0x56, 0x09, 0x79, 0x1D, 0x5A, 0x5F, \
                                          0x4A, 0x5C, 0xFE, 0x89, 0x56, 0xEC, \
                                          0xE6, 0xF7, 0x92, 0x21, 0xAC, 0x93, \
                                          0x99, 0x10, 0x51, 0x82, 0xF4, 0xDD, \
                                          0x84, 0x07, 0x50, 0x99, 0xE7, 0xC2, \
                                          0xF1, 0xC8 }

#define APP_PUBLIC_KEY_Y                { 0x40, 0x84, 0xB4, 0xA6, 0x08, 0x67, \
                                          0xFD, 0xAC, 0x81, 0x5D, 0xB0, 0x41, \
                                          0x27, 0x75, 0x9B, 0xA7, 0x92, 0x57, \
                                          0x0C, 0x44, 0xB1, 0x57, 0x7C, 0x76, \
                                          0x5B, 0x56, 0xF0, 0xBA, 0x03, 0xF4, \
                                          0xAA, 0x67 }

/* List of message handlers that are used by the Bluetooth application
 * manager */
#define SEC_MESSAGE_HANDLER_LIST                                      \
    DEFINE_MESSAGE_HANDLER(GAPC_BOND_REQ_IND, GAPC_BondReqInd),       \
    DEFINE_MESSAGE_HANDLER(GAPC_BOND_IND, GAPC_BondInd),              \
    DEFINE_MESSAGE_HANDLER(GAPC_ENCRYPT_REQ_IND, GAPC_EncryptReqInd), \
    DEFINE_MESSAGE_HANDLER(GAPC_ENCRYPT_IND, GAPC_EncryptInd),        \
    DEFINE_MESSAGE_HANDLER(GAPM_ADDR_SOLVED_IND, GAPM_AddrSolvedInd)  \

/* ----------------------------------------------------------------------------
 * Global variables and types
 *--------------------------------------------------------------------------*/

/* Bluetooth Device Address */
extern uint8_t bdaddr[BDADDR_LENGTH];
extern uint8_t bdaddr_type;

/* ----------------------------------------------------------------------------
 * Function prototype definitions
 * --------------------------------------------------------------------------*/
extern int GAPC_BondReqInd(ke_msg_id_t const msgid,
                           struct gapc_bond_req_ind const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id);

extern int GAPC_BondInd(ke_msg_id_t const msgid,
                        struct gapc_bond_ind const *param,
                        ke_task_id_t const dest_id,
                        ke_task_id_t const src_id);

extern int GAPC_BondCfm(ke_msg_id_t const msg_id,
                        struct gapc_bond_cfm const *param,
                        ke_task_id_t const dest_id,
                        ke_task_id_t const src_id);

extern int GAPC_EncryptReqInd(ke_msg_id_t const msg_id,
                              struct gapc_encrypt_req_ind const *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id);

extern int GAPC_EncryptInd(ke_msg_id_t const msg_id,
                           struct gapc_encrypt_ind const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id);

extern int GAPM_AddrSolvedInd(ke_msg_id_t const msg_id,
                              struct gapm_addr_solved_ind const *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id);

/* Bluetooth bonding support functions */
extern bool BondList_Add(BondInfo_Type *bond_info);

extern bool BondList_FindByAddr(const uint8_t *addr,
                                const uint8_t addr_type,
                                BondInfo_Type *result);

extern bool BondList_FindByIRK(const uint8_t *irk, BondInfo_Type *result);

extern uint8_t BondList_GetIRKs(struct gap_sec_key *irkList);

extern bool BondList_Remove(BondInfo_Type *target);

extern bool BondList_RemoveAll(void);

extern uint8_t BondList_Size(void);

extern void NVR2_WriteEnable(bool enable);

/* ----------------------------------------------------------------------------
 * Close the 'extern "C"' block
 * ------------------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif    /* ifdef __cplusplus */

#endif    /* BLE_SEC_H */
