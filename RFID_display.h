/*
 * RFID_display.h
 *
 *  Created on: 02-Sep-2022
 *      Author: Mukarram Saeed
 */

#ifndef RFID_DISPLAY_H_
#define RFID_DISPLAY_H_


#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <uartdrv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

/*******************************************************************************
 *  TYPEDEFS
 ******************************************************************************/



  typedef enum
  {
    LOGO0_PAGE,
    LOGO1_PAGE,
    LOGO2_PAGE,
    LOGO3_PAGE,
    ID_PAGE,
    DATA_PAGE,
    SUCCESS_PAGE,
    ERROR_PAGE,
    PROCESS_PAGE
  }RFID_Lcd_Ui_T;

  typedef struct
  {
    char val_00[20];
    char val_01[20];
    char val_10[15];
    char val_11[15];
    char val_20[15];
    char val_21[15];
  } Layout_70_t;

  typedef struct
  {
    uint32_t code;
    char msg[40];
  } Notification_t;

  typedef struct
  {
    char type[15];
    char value[30];
  } TagInfo_t;

  extern Layout_70_t layout_70;
  extern uint8_t currentPage;
  extern uint8_t previousPage;


  extern char *const UIs_Cmd[];
/*********************************************************************
 * FUNCTIONS
 */
  void RFID_display_Init(void);
  void RFID_display_Write(char *iData,uint16_t iLen);
  void RFID_HMIUart_Write(char *iData);
  void HexToStr(uint16_t num, uint8_t *buf);
  void displayLogo(RFID_Lcd_Ui_T iUI);
  void displayIdPage(RFID_Lcd_Ui_T iUI,char *ieeeAddr,uint16_t shortAddr,uint16_t boxId);
  void displayDataPage(RFID_Lcd_Ui_T iUI,Layout_70_t data);
  void displaySuccessPage(RFID_Lcd_Ui_T iUI, Notification_t info);
  void displayErrorPage(RFID_Lcd_Ui_T iUI, Notification_t info);
  void displayProcessPage(RFID_Lcd_Ui_T iUI, TagInfo_t tag);


/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* RFID_DISPLAY_H_ */
