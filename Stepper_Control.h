/********************************************************************************
  * @file    Stepper_Control.h
  * @author  Donovan Bidlack
  * @brief   header file that contains all of the API's needed for running a test
           that requires stepper motor control. 
********************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STEPPER_CONTROL_H__
#define __STEPPER_CONTROL_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Stepper Control Defines */
#define STEPPER_A_HANDLE                                        htim1
#define STEPPER_B_HANDLE                                        htim2
#define STEPPER_C_HANDLE                                        htim15 

#define STEPPER_A_PIN                                           Stepper_A_Enable_Pin
#define STEPPER_B_PIN                                           Stepper_B_Enable_Pin
#define STEPPER_C_PIN                                           Stepper_C_Enable_Pin  

#define STEPPER_A_PORT                                          Stepper_A_Enable_GPIO_Port
#define STEPPER_B_PORT                                          Stepper_B_Enable_GPIO_Port
#define STEPPER_C_PORT                                          Stepper_C_Enable_GPIO_Port  

#define STEP_DELAY                                              10
#define NUMBER_OF_STEPS                                         30
#define SP_005FPS                                               398
#define SP_01FPS                                                198
#define SP_02FPS                                                99
#define SP_03FPS                                                66
#define SP_04FPS                                                49
#define SP_05FPS                                                38
#define SP_06FPS                                                32
#define SP_07FPS                                                27
#define SP_08FPS                                                24
#define SP_09FPS                                                21
#define SP_10FPS                                                19
#define SP_11FPS                                                17
#define SP_12FPS                                                15
#define SP_13FPS                                                14
#define SP_14FPS                                                13
#define SP_15FPS                                                12
#define SP_16FPS                                                11
#define SP_17FPS                                                11
#define SP_18FPS                                                10

#define Stepper_A_Enable() HAL_GPIO_WritePin(STEPPER_A_PORT, STEPPER_A_PIN, GPIO_PIN_RESET)
#define Stepper_B_Enable() HAL_GPIO_WritePin(STEPPER_B_PORT, STEPPER_B_PIN, GPIO_PIN_RESET)
#define Stepper_C_Enable() HAL_GPIO_WritePin(STEPPER_C_PORT, STEPPER_C_PIN, GPIO_PIN_RESET)
#define Stepper_A_Disable() HAL_GPIO_WritePin(STEPPER_A_PORT, STEPPER_A_PIN, GPIO_PIN_SET)
#define Stepper_B_Disable() HAL_GPIO_WritePin(STEPPER_B_PORT, STEPPER_B_PIN, GPIO_PIN_SET)
#define Stepper_C_Disable() HAL_GPIO_WritePin(STEPPER_C_PORT, STEPPER_C_PIN, GPIO_PIN_SET)

/* Stepper Control Function Prototypes  --------------------------------------*/

/**********************************************
  Name: Stepper_Init
  Description: initializes the devices needed.
**********************************************/
void Stepper_Init( void );

/**********************************************
  Name: Stepper_Test_Process
  Description: runs the Test Process that
        requires stepper motor control.  
**********************************************/
void Stepper_Test_Process( void );

/**********************************************
  Name: Stepper_Slow_Acceleration_until_Set_Velocity
  Description: increases the velocity of the
        stepper motor from zero velocity to 
        final_velocity. This is done in small
        incriments because if you immediately
        increase the velocity on the stepper
        motors the motor will lock up. 
**********************************************/
void Stepper_Slow_Acceleration_until_Set_Velocity( uint16_t final_velocity, TIM_HandleTypeDef timer_handle );

/**********************************************
  Name: Stepper_Increase_Velocity
  Description: increases the velocity of the
        stepper motor from intial_velocity to 
        final_velocity. This is done in small
        incriments because if you immediately
        increase the velocity on the stepper
        motors the motor will lock up.
**********************************************/
void Stepper_Increase_Velocity( uint16_t initial_velocity, uint16_t final_velocity, TIM_HandleTypeDef timer_handle );

/**********************************************
  Name: Stepper_Set_Velocity
  Description: Changes the velocity value of the 
      PWM for motor control (0 to 4095).
**********************************************/
void Stepper_Set_Velocity(uint16_t value,  TIM_HandleTypeDef timer_handle);

#endif /* __STEPPER_CONTROL_H__ */
