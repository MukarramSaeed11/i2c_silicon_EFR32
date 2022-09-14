/*
 * RFID_display.c
 *
 *  Created on: 02-Sep-2022
 *      Author: Mukarram Saeed
 */

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "RFID_display.h"

Layout_70_t layout_70;
uint8_t currentPage = LOGO0_PAGE;
uint8_t previousPage = LOGO0_PAGE;
char screenBuffer[400];
DEFINE_BUF_QUEUE(EMDRV_UARTDRV_MAX_CONCURRENT_RX_BUFS, rxBufferQueue);
DEFINE_BUF_QUEUE(EMDRV_UARTDRV_MAX_CONCURRENT_TX_BUFS, txBufferQueue);
UARTDRV_HandleData_t handleData;
UARTDRV_Handle_t handle = &handleData;


char *const UIs_Cmd[] =
      { "FSIMG(2097152,0,0,160,128,0);\r\n",
          "FSIMG(2138112,0,0,160,128,0);\r\n",
          "FSIMG(2179072,0,0,160,128,0);\r\n",
          "FSIMG(2220032,0,0,160,128,0);\r\n",
          "DIR(1);CLR(15);BOXF(10,105,148,120,0);DC16(15,105,'%s',15);BOXF(60,78,100,95,0);DC16(65,78,'%.4X',15);BOXF(45,40,115,67,0);DC24(50,40,'%X',15);DC16(100,10,'v_4.2.0',0);\r\n",
          "DIR(1);CLR(15);FSIMG(2260992,0,0,160,128,0);BOXF(25,23,75,38,15);DC16(22,23,'%s',21);BOXF(105,23,155,38,15);DC16(102,23,'%s',21);BOXF(25,60,75,75,15);DCV16(25,60,'%s',21);BOXF(105,60,155,75,15);DCV16(105,60,'%s',21);BOXF(25,100,75,115,15);DC16(22,100,'%s',21);BOXF(105,100,155,115,15);DCV16(105,100,'%s',21);\r\n",
          "FSIMG(2342912,0,0,160,128,0);DC24(64,80,'%d',15);DC16(0,110,'%s',15);\r\n",
          "FSIMG(2383872,0,0,160,128,0);DC24(64,80,'%d',15);DC16(0,110,'%s',15);\r\n",
          "FSIMG(2301952,0,0,160,128,0);DC16(5,75,'%s',15);DC16(1,95,'%s',15);\r\n", };



void
HexToStr (uint16_t num, uint8_t *buf)
{
  char c, i;
  uint8_t *p, rst[5];

  p = rst;
  for (i = 0; i < 5; i++, p++)
    {
      c = num % 16;  // Isolate a digit
      *p = c + ((c < 10) ? '0' : '7');  // Convert to Ascii
      num /= 16;
      if (!num)
        {
          break;
        }
    }

  for (c = 3 - i; c > 0; c--)
    {
      *buf++ = '0';  // Reverse character order
    }
  for (c = 0; c <= i; c++)
    {
      *buf++ = *p--;  // Reverse character order
    }

  *buf = '\0';
}

void
RFID_display_Init ()
{

  UARTDRV_InitUart_t init;
  init.port = USART2;
  init.baudRate = 115200;
  init.txPort = gpioPortC;
  init.txPin = 2;
  init.rxPort = gpioPortC;
  init.rxPin = 3;
  init.stopBits = usartStopbits1;
  init.parity = usartNoParity;
  init.oversampling = usartOVS6;
  init.fcType = uartdrvFlowControlNone;
  init.mvdis = false;
  init.uartNum = 2;
  init.rxQueue = (UARTDRV_Buffer_FifoQueue_t*) &rxBufferQueue;
  init.txQueue = (UARTDRV_Buffer_FifoQueue_t*) &txBufferQueue;
  UARTDRV_InitUart (handle, &init);
}

void
RFID_display_Write (char *iData, uint16_t iLen)
{
  uint8_t pData[3] =
    { 0 };
  pData[0] = 0xFF;
  pData[1] = 0xFF;
  pData[2] = 0xFF;

  UARTDRV_TransmitB (handle, (uint8_t*) iData, iLen);
  UARTDRV_TransmitB (handle, pData, 3);
}
void
RFID_HMIUart_Write (char *iData)
{
  UARTDRV_TransmitB (handle, (uint8_t*) iData, strlen (iData));
}

void
displayLogo (RFID_Lcd_Ui_T iUI)
{
  RFID_HMIUart_Write (UIs_Cmd[iUI]);
  if (iUI == LOGO3_PAGE)
    currentPage = LOGO0_PAGE;
  else
    currentPage = iUI + 1;
  //osal_start_timerEx( RFID_Router_TaskID,RFID_HMI_DISP_EVT,200 );
}
void
displayIdPage (RFID_Lcd_Ui_T iUI, char *ieeeAddr, uint16_t shortAddr,
               uint16_t boxId)
{
  sprintf (screenBuffer, UIs_Cmd[iUI], ieeeAddr, shortAddr, boxId);
  RFID_HMIUart_Write (screenBuffer);
}

void
displayDataPage (RFID_Lcd_Ui_T iUI, Layout_70_t data)
{
  sprintf (screenBuffer, UIs_Cmd[iUI], data.val_00, data.val_01, data.val_10,
           data.val_11, data.val_20, data.val_21);
  RFID_HMIUart_Write (screenBuffer);
}
void
displaySuccessPage (RFID_Lcd_Ui_T iUI, Notification_t info)
{
  sprintf (screenBuffer, UIs_Cmd[iUI], info.code, info.msg);
  RFID_HMIUart_Write (screenBuffer);
  currentPage = previousPage;
  //osal_start_timerEx( RFID_Router_TaskID,RFID_HMI_DISP_EVT,1000 );
}
void
displayErrorPage (RFID_Lcd_Ui_T iUI, Notification_t info)
{
  sprintf (screenBuffer, UIs_Cmd[iUI], info.code, info.msg);
  RFID_HMIUart_Write (screenBuffer);
  currentPage = previousPage;
  //osal_start_timerEx( RFID_Router_TaskID,RFID_HMI_DISP_EVT,2000 );
}
void
displayProcessPage (RFID_Lcd_Ui_T iUI, TagInfo_t tag)
{
  sprintf (screenBuffer, UIs_Cmd[iUI], tag.type, tag.value);
  RFID_HMIUart_Write (screenBuffer);
  //currentPage = previousPage;
  //displayDataPage(DATA_PAGE,layout_70);
  //osal_start_timerEx( RFID_Router_TaskID,RFID_HMI_DISP_EVT,2000 );
}

