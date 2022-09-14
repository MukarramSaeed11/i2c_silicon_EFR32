/*
 * rtc3231.c
 *
 *  Created on: 30-Aug-2022
 *      Author: Mukarram Saeed
 */


#include "../code1/rtc3231.h"

void rtc_init()
{
  write_to_rtc(0x0E, 0x00);  //control reg
  write_to_rtc(0x0F, 0x08);
}

/*                       Conversion Functions                                 */
uint16_t  bcd_to_dec(uint8_t value)
{
  return (value >> 4) * 10 + (value & 0x0F);
}

uint16_t dec_to_bcd(uint8_t value)
{
  return ((value / 10) << 4) + (value % 10);
}

/*                       Write Function to i2c                                */

void write_to_rtc(uint8_t which_addr, uint8_t val)
{
  //int b[6];
  I2C_TransferSeq_TypeDef i2cTransfer;
  I2C_TransferReturn_TypeDef result;
  uint8_t txBuffer[2];
  txBuffer[0]=which_addr;
  txBuffer[1]=val;

  // Initializing I2C transfer
  i2cTransfer.addr          = RTC_SLAVE_ADDRESS<<1;
  i2cTransfer.flags         = I2C_FLAG_WRITE;
  i2cTransfer.buf[0].data   = txBuffer;
  i2cTransfer.buf[0].len    = 2;
  result=I2CSPM_Transfer(I2C0, &i2cTransfer);
  if (result != i2cTransferDone)    {
   //   UARTDRV_Transmit (handle,"transfer failed\n\r",sizeof("transfer failed\n\r"),uart_callback);
    }
  //sprintf(b,"%d\n\r",result);
 // UARTDRV_Transmit (handle,b,sizeof(b),uart_callback);
}

/*                       Read Values from i2c                                 */

uint8_t read_from_rtc(uint8_t Addr)
{
  // Transfer structure
  I2C_TransferSeq_TypeDef i2cTransfer;
  I2C_TransferReturn_TypeDef result;

  //uint8_t txBuffer[1];
 // txBuffer[0] = Addr;

  uint8_t rxBuffer[1];

  // Initializing I2C transfer
  i2cTransfer.addr          = RTC_SLAVE_ADDRESS<<1;
  i2cTransfer.flags         = I2C_FLAG_WRITE_READ;
  i2cTransfer.buf[0].data   = &Addr;
  i2cTransfer.buf[0].len    = 1;
  i2cTransfer.buf[1].data   = rxBuffer;
  i2cTransfer.buf[1].len    = 1;
  result=I2CSPM_Transfer(I2C0, &i2cTransfer);
  if (result != i2cTransferDone)    {
   //   UARTDRV_Transmit (handle,"transfer Read failed\n\r",sizeof("transfer Read failed\n\r"),uart_callback);
    }
  return rxBuffer[0];
}

/*                        Setting time of RTC                                 */

void set_time(rtc_time_info_t rtc_time)
{
   uint8_t temp = 0;
   write_to_rtc(RTC_REG_SECOND, dec_to_bcd(rtc_time.sec));
   write_to_rtc(RTC_REG_MINUTE, dec_to_bcd(rtc_time.mins));
   switch(rtc_time.hour_twelve_twentyfour)
    {
      case 1:
       {
         switch(rtc_time.am_pm)
          {
            case 1:
             {
               temp = 0x60;
               break;
             }
             default:
              {
                temp = 0x40;
                break;
              }
           }

          write_to_rtc(RTC_REG_HOUR, ((temp | (dec_to_bcd(rtc_time.hours)))));
          break;
        }
       default:
        {
          write_to_rtc(RTC_REG_HOUR,(dec_to_bcd(rtc_time.hours)));
          break;
        }
    }
}

/*                        Setting Date of RTC                                 */

void set_date(rtc_date_info_t rtc_date)
{
   write_to_rtc(RTC_REG_DAY,  (dec_to_bcd(rtc_date.day)));
   write_to_rtc(RTC_REG_DATE, (dec_to_bcd(rtc_date.date)));
   write_to_rtc(RTC_REG_MONTH,(dec_to_bcd(rtc_date.month)));
   write_to_rtc(RTC_REG_YEAR, (dec_to_bcd(rtc_date.year)));
}

/*                        Get Time from RTC                                   */

rtc_time_info_t get_time()
{
   rtc_time_info_t rtc_info;
   rtc_info.mins = bcd_to_dec(read_from_rtc(RTC_REG_MINUTE));
   rtc_info.sec = bcd_to_dec(read_from_rtc(RTC_REG_SECOND));

   switch(rtc_info.hour_twelve_twentyfour)
    {
      case 1:
       {
         rtc_info.hours = bcd_to_dec(0x1F & (read_from_rtc(RTC_REG_HOUR)));
         break;
       }
      default:
       {
         rtc_info.hours = bcd_to_dec(0x3F & (read_from_rtc(RTC_REG_HOUR)));
         break;
       }
    }
   //UARTprintf(" %u : %u : %u \n",rtc_info.hours,rtc_info.mins,rtc_info.sec);
   return rtc_info;
}

/*                         Get Date                                           */

rtc_date_info_t get_date()
{
   rtc_date_info_t rtc_info;
   rtc_info.day=bcd_to_dec(0x07&(read_from_rtc(RTC_REG_DAY)));
   rtc_info.date=bcd_to_dec(0x3F&(read_from_rtc(RTC_REG_DATE)));
   rtc_info.month=bcd_to_dec(0x1F&(read_from_rtc(RTC_REG_MONTH)));
   rtc_info.year=bcd_to_dec(read_from_rtc(RTC_REG_YEAR));
   //UARTprintf(" %u / %u / %u / %u \n",rtc_info.day,rtc_info.date,rtc_info.month,rtc_info.year);
   return rtc_info;
}
