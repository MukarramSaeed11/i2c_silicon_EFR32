/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef APP_H
#define APP_H


// RFID Sector Block Definition
#define RFID_SECTOR             0x01
#define RFID_BLOCK_ADDRESS      0x05
#define RFID_TRAILER_BLOCK      0x07

// Card Types Definition
#define RESET_NWK_CARD_TYPE     0x01
#define PAN_CARD_TYPE           0x02
#define DST_ADDR_CARD_TYPE      0x03
#define COLOR_CARD_TYPE         0x04
#define EMP_CARD_TYPE           0x05
#define OPR_CARD_TYPE           0x06
#define BUNDLE_CARD_TYPE        0x07
#define MAC_CARD_TYPE           0x08
#define CUT_CARD_TYPE           0x09
#define UNT_CARD_TYPE           0x0A

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void);

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void);

#endif  // APP_H
