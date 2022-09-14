#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[CMU]
// [CMU]$

// $[LFXO]
// [LFXO]$

// $[PRS.ASYNCH0]
// [PRS.ASYNCH0]$

// $[PRS.ASYNCH1]
// [PRS.ASYNCH1]$

// $[PRS.ASYNCH2]
// [PRS.ASYNCH2]$

// $[PRS.ASYNCH3]
// [PRS.ASYNCH3]$

// $[PRS.ASYNCH4]
// [PRS.ASYNCH4]$

// $[PRS.ASYNCH5]
// [PRS.ASYNCH5]$

// $[PRS.ASYNCH6]
// [PRS.ASYNCH6]$

// $[PRS.ASYNCH7]
// [PRS.ASYNCH7]$

// $[PRS.ASYNCH8]
// [PRS.ASYNCH8]$

// $[PRS.ASYNCH9]
// [PRS.ASYNCH9]$

// $[PRS.ASYNCH10]
// [PRS.ASYNCH10]$

// $[PRS.ASYNCH11]
// [PRS.ASYNCH11]$

// $[PRS.SYNCH0]
// [PRS.SYNCH0]$

// $[PRS.SYNCH1]
// [PRS.SYNCH1]$

// $[PRS.SYNCH2]
// [PRS.SYNCH2]$

// $[PRS.SYNCH3]
// [PRS.SYNCH3]$

// $[GPIO]
// GPIO SWV on PA03
#define GPIO_SWV_PORT                            gpioPortA
#define GPIO_SWV_PIN                             3

// [GPIO]$

// $[TIMER0]
// TIMER0 CC0 on PC00
#define TIMER0_CC0_PORT                          gpioPortC
#define TIMER0_CC0_PIN                           0

// TIMER0 CC1 on PC01
#define TIMER0_CC1_PORT                          gpioPortC
#define TIMER0_CC1_PIN                           1

// TIMER0 CC2 on PB00
#define TIMER0_CC2_PORT                          gpioPortB
#define TIMER0_CC2_PIN                           0

// [TIMER0]$

// $[TIMER1]
// [TIMER1]$

// $[TIMER2]
// [TIMER2]$

// $[TIMER3]
// [TIMER3]$

// $[USART0]
// [USART0]$

// $[USART1]
// [USART1]$

// $[USART2]
// USART2 RX on PC03
#define USART2_RX_PORT                           gpioPortC
#define USART2_RX_PIN                            3

// USART2 TX on PC02
#define USART2_TX_PORT                           gpioPortC
#define USART2_TX_PIN                            2

// [USART2]$

// $[I2C1]
// [I2C1]$

// $[LETIMER0]
// [LETIMER0]$

// $[ACMP0]
// [ACMP0]$

// $[ACMP1]
// [ACMP1]$

// $[I2C0]
// I2C0 SCL on PD02
#define I2C0_SCL_PORT                            gpioPortD
#define I2C0_SCL_PIN                             2

// I2C0 SDA on PD03
#define I2C0_SDA_PORT                            gpioPortD
#define I2C0_SDA_PIN                             3

// [I2C0]$

// $[PTI]
// [PTI]$

// $[MODEM]
// [MODEM]$

// $[CUSTOM_PIN_NAME]
// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

