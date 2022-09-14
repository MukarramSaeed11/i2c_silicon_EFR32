/***************************************************************************//**
 * @file
 * @brief PWM Driver
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef PWM_INIT_GREEN_CONFIG_H
#define PWM_INIT_GREEN_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>PWM configuration

// <o SL_PWM_GREEN_FREQUENCY> PWM frequency [Hz]
// <i> Default: 10000
#define SL_PWM_GREEN_FREQUENCY       10000

// <o SL_PWM_GREEN_POLARITY> Polarity
// <PWM_ACTIVE_HIGH=> Active high
// <PWM_ACTIVE_LOW=> Active low
// <i> Default: PWM_ACTIVE_HIGH
#define SL_PWM_GREEN_POLARITY        PWM_ACTIVE_HIGH
// </h> end pwm configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <timer channel=OUTPUT> SL_PWM_GREEN
// $[TIMER_SL_PWM_GREEN]
#define SL_PWM_GREEN_PERIPHERAL                  TIMER0
#define SL_PWM_GREEN_PERIPHERAL_NO               0

#define SL_PWM_GREEN_OUTPUT_CHANNEL              0
// TIMER0 CC0 on PC00
#define SL_PWM_GREEN_OUTPUT_PORT                 gpioPortC
#define SL_PWM_GREEN_OUTPUT_PIN                  0
// [TIMER_SL_PWM_GREEN]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // PWM_INIT_GREEN_CONFIG_H
