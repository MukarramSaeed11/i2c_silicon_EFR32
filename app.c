#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "sl_simple_rgb_pwm_led.h"
#include "sl_simple_rgb_pwm_led_inst0_config.h"
#include "sl_simple_rgb_pwm_led_instances.h"
#include "sl_udelay.h"
#include "i2c_init.h"
#include "ssd1306.h"
#include "rtc3231.h"
#include "RFID_display.h"
#include "rc522.h"


MFRC522_Uid_T UID;
Card_f card;
TagInfo_t tagInfo;
Notification_t notify;

CardData emp;
CardData opr;
CardData mac;
CardData unt;
CardData bnd;
CardData cut;
static char rfPackBuffer[70] =
  { 0 };

extern UARTDRV_Handle_t handle;

rtc_time_info_t rtc_time;
void
sset_time ()
{
  rtc_time.hours = 12;
  rtc_time.mins = 12;
  rtc_time.sec = 12;
  set_time (rtc_time);
}


uint8_t p=10;
void
RFID_card_detect_cb ()
{
  currentPage = DATA_PAGE;
  if(currentPage == DATA_PAGE || currentPage == ID_PAGE)
  {
    previousPage = currentPage;
  }
  if (card.buffer[4] == EMP_CARD_TYPE)
    {
      emp.c[0] = card.buffer[0];
      emp.c[1] = card.buffer[1];
      emp.c[2] = card.buffer[2];
      emp.c[3] = card.buffer[3];


      sprintf (layout_70.val_00, "%u", p);
      sprintf (layout_70.val_01, "%u", p);
      sprintf (layout_70.val_10, "%u", p);
      sprintf (layout_70.val_11, "%u", p);
      sprintf (layout_70.val_20, "%u", emp.n);
      sprintf (layout_70.val_21, "%u", p);
      layout_70.val_20[7] = '\0';

      strcpy (tagInfo.type, "Worker:");
      sprintf (tagInfo.value, "%u", emp.n);
      displayProcessPage (PROCESS_PAGE, tagInfo);
      sl_sleeptimer_delay_millisecond(700);
      displayDataPage(DATA_PAGE,layout_70);

      sprintf (rfPackBuffer, "EMP:%u", emp.n);
      SSD1306_DrawText (0, 48, rfPackBuffer, 2);
    }
  else if (card.buffer[4] == OPR_CARD_TYPE)  //new addition
    {
      opr.c[0] = card.buffer[0];
      opr.c[1] = card.buffer[1];
      opr.c[2] = card.buffer[2];
      opr.c[3] = card.buffer[3];

      sprintf (layout_70.val_01, "%d", opr.n);
      layout_70.val_01[7] = '\0';

      strcpy (tagInfo.type, "Operation:");
      sprintf (tagInfo.value, "%u", opr.n);
      displayProcessPage (PROCESS_PAGE, tagInfo);

      sprintf (rfPackBuffer, "OPR:%u", opr.n);
      SSD1306_DrawText (0, 48, rfPackBuffer, 2);
    }

  else if (card.buffer[4] == UNT_CARD_TYPE)  //new addition
    {
      unt.c[0] = card.buffer[0];
      unt.c[1] = card.buffer[1];
      unt.c[2] = card.buffer[2];
      unt.c[3] = card.buffer[3];

      sprintf (layout_70.val_00, "%d/%d", mac.n, unt.n);
      layout_70.val_00[7] = '\0';

      strcpy (tagInfo.type, "Line:");
      sprintf (tagInfo.value, "%u", unt.n);
      displayProcessPage (PROCESS_PAGE, tagInfo);

      sprintf (rfPackBuffer, "UNT:%u", unt.n);
      SSD1306_DrawText (0, 48, rfPackBuffer, 2);
    }
  else if (card.buffer[4] == MAC_CARD_TYPE)
    {
      mac.c[0] = card.buffer[0];
      mac.c[1] = card.buffer[1];
      mac.c[2] = card.buffer[2];
      mac.c[3] = card.buffer[3];

      sprintf (layout_70.val_00, "%d/%d", mac.n, unt.n);
      layout_70.val_00[7] = '\0';

      strcpy (tagInfo.type, "Machine:");
      sprintf (tagInfo.value, "%u", mac.n);
      displayProcessPage (PROCESS_PAGE, tagInfo);

      sprintf (rfPackBuffer, "MACH:%u", mac.n);
      SSD1306_DrawText (0, 48, rfPackBuffer, 2);
    }
  else if (card.buffer[4] == BUNDLE_CARD_TYPE)
    {
      bnd.c[0] = card.buffer[0];
      bnd.c[1] = card.buffer[1];
      bnd.c[2] = card.buffer[2];
      bnd.c[3] = card.buffer[3];

      strcpy (tagInfo.type, "Bundle:");
      sprintf (tagInfo.value, "%u", bnd.n);
      displayProcessPage (PROCESS_PAGE, tagInfo);

      sprintf (rfPackBuffer, "BUN:%u", bnd.n);
      SSD1306_DrawText (0, 24, rfPackBuffer, 2);
    }

  else if (card.buffer[4] == CUT_CARD_TYPE)
    {
      cut.c[0] = card.buffer[0];
      cut.c[1] = card.buffer[1];
      cut.c[2] = card.buffer[2];
      cut.c[3] = card.buffer[3];

      strcpy (tagInfo.type, "Cut:");
      sprintf (tagInfo.value, "%u", bnd.n);
      displayProcessPage (PROCESS_PAGE, tagInfo);

      sprintf (rfPackBuffer, "CUT:%u", cut.n);
      SSD1306_DrawText (0, 48, rfPackBuffer, 2);
    }
  else if (card.buffer[4] == COLOR_CARD_TYPE)
     {
       if(card.buffer[0]==255)
         {
           sl_led_set_rgb_color(&sl_simple_rgb_pwm_led_inst0, 65535, 0, 0);
         }
       if(card.buffer[1]==255)
         {  sl_led_set_rgb_color(&sl_simple_rgb_pwm_led_inst0, 0, 65535, 0);
         }
       if(card.buffer[2]==255)
         {  sl_led_set_rgb_color(&sl_simple_rgb_pwm_led_inst0, 0, 0, 65535);
         }
     }
}


void app_init(void)
{
  sset_time ();
  card.blockAddr = RFID_BLOCK_ADDRESS;
  card.sector = RFID_SECTOR;
  card.trailerBlock = RFID_TRAILER_BLOCK;
  card.size = 18;
  i2c_init ();
  RFID_display_Init ();
  SSD1306_Begin (SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
  MFRC522_Init ();
  rtc_init ();
  UARTDRV_TransmitB (handle, (uint8_t*) "DIR(1);CLR(15);\r\n",
                     strlen ("DIR(1);CLR(15);\r\n"));
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  SSD1306_DisplayTime (rtc_time = get_time ());

  if (MFRC522_IsNewCardPresent () == STATUS_OK)
    {
      if (MFRC522_ReadCard (&UID) == STATUS_OK)
        {
          if (Authenticate (card.trailerBlock, &UID) == STATUS_OK)
            {
              card.size = 18;
              MIFARE_Read (&card);
              PICC_HaltA ();
              PCD_StopCrypto1 ();
              RFID_card_detect_cb ();
            }
        }
    }

}
