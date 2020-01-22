/********************************************************************************
  * @file    Stepper_Control.c
  * @author  Donovan Bidlack
  * @brief   c file that contains all of the API's needed for running a test
           that requires stepper motor control. 

setup: Stepper_Init() needs to be called after STM Inits in Main. Then call 
  Stepper_Test_Process() when you want to run the test. Setpper Ports and Pins
  needs to be set in the header file. This should be a GPIO that goes into the 
  Stepper Motor Controller IC nEnable.

  Timer Settings for PWMs:
    timer clock:                80MHz
    PSC:                        4000
    Counter Mode:               Up
    ARR:                        200
    CKD:                        No Division
    RCR:                        0
    Auto-Preload:               Disabled
    Slave Mode Controller:      Slave mode disable
    PWM Generation Channel1:    PWM mode 1
    Pulse:                      100
    Fast Mode:                  Disable
    CH Polarity:                High
    CH Idle State:              Reset

********************************************************************************/

#include "Stepper_Control.h"

/**********************************************
  Name: Stepper_Init
  Description: initializes the devices needed.
**********************************************/
void Stepper_Init( void )
{      
    HAL_TIM_Base_Start_IT(&STEPPER_A_HANDLE);
    HAL_TIM_Base_Start_IT(&STEPPER_B_HANDLE);
    HAL_TIM_Base_Start_IT(&STEPPER_C_HANDLE);
    
    HAL_TIM_PWM_Start(&STEPPER_A_HANDLE, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&STEPPER_B_HANDLE, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&STEPPER_C_HANDLE, TIM_CHANNEL_1);
    
    Stepper_A_Disable();
    Stepper_B_Disable();
    Stepper_C_Disable();
}

/**********************************************
  Name: Stepper_Test_Process
  Description: runs the Test Process that
        requires stepper motor control.  
**********************************************/
void Stepper_Test_Process( void )
{     
    while(HAL_GPIO_ReadPin(Start_Run_GPIO_Port, Start_Run_Pin) == GPIO_PIN_RESET);
    Stepper_A_Enable();
    Stepper_Slow_Acceleration_until_Set_Velocity(370, STEPPER_A_HANDLE);
    Stepper_B_Enable();
    Stepper_Slow_Acceleration_until_Set_Velocity(SP_005FPS, STEPPER_B_HANDLE);
    Stepper_C_Enable();
    Stepper_Slow_Acceleration_until_Set_Velocity(SP_005FPS, STEPPER_C_HANDLE);
        
    while(HAL_GPIO_ReadPin(Start_Run_GPIO_Port, Start_Run_Pin) == GPIO_PIN_SET)
    {
    HAL_Delay(1000);
    Stepper_A_Disable();
    Stepper_Set_Velocity(4000, STEPPER_A_HANDLE);
    HAL_Delay(125);
    Stepper_A_Enable();
    Stepper_Slow_Acceleration_until_Set_Velocity(370, STEPPER_A_HANDLE);
    }
        
    Stepper_A_Disable();
    Stepper_B_Disable();
    Stepper_C_Disable();
}

/**********************************************
  Name: Stepper_Slow_Acceleration_until_Set_Velocity
  Description: increases the velocity of the
        stepper motor from zero velocity to 
        final_velocity. This is done in small
        incriments because if you immediately
        increase the velocity on the stepper
        motors the motor will lock up. 
**********************************************/
void Stepper_Slow_Acceleration_until_Set_Velocity( uint16_t final_velocity, TIM_HandleTypeDef timer_handle )
{
    uint16_t current_velocity = 2000;
    Stepper_Set_Velocity(current_velocity, timer_handle);
    while(current_velocity > final_velocity)
    {
        HAL_Delay(STEP_DELAY);
        if(current_velocity < 80)
        {
            if(current_velocity < 10)
            {
                Stepper_Set_Velocity(final_velocity, timer_handle);
                return;
            }
            current_velocity -= 1;
            Stepper_Set_Velocity(current_velocity, timer_handle);
        }
        else if(current_velocity < 100)
        {
            current_velocity -= 10;
            Stepper_Set_Velocity(current_velocity, timer_handle);
        }
        else
        {
            current_velocity /= 2;
            Stepper_Set_Velocity(current_velocity, timer_handle);
        }
    } 
    Stepper_Set_Velocity(final_velocity, timer_handle);
}

/**********************************************
  Name: Stepper_Increase_Velocity
  Description: increases the velocity of the
        stepper motor from intial_velocity to 
        final_velocity. This is done in small
        incriments because if you immediately
        increase the velocity on the stepper
        motors the motor will lock up.
**********************************************/
void Stepper_Increase_Velocity( uint16_t initial_velocity, uint16_t final_velocity, TIM_HandleTypeDef timer_handle )
{
    uint16_t current_velocity = initial_velocity;
    Stepper_Set_Velocity(current_velocity, timer_handle);
    while(current_velocity > final_velocity)
    {
        HAL_Delay(STEP_DELAY);
        if(current_velocity < 80)
        {
            if(current_velocity < 10)
            {
                Stepper_Set_Velocity(final_velocity, timer_handle);
                return;
            }
            current_velocity -= 1;
            Stepper_Set_Velocity(current_velocity, timer_handle);
        }
        else if(current_velocity < 100)
        {
            current_velocity -= 10;
            Stepper_Set_Velocity(current_velocity, timer_handle);
        }
        else
        {
            current_velocity /= 2;
            Stepper_Set_Velocity(current_velocity, timer_handle);
        }
    } 
    Stepper_Set_Velocity(final_velocity, timer_handle);
}

/**********************************************
  Name: Stepper_Set_Velocity
  Description: Changes the velocity value of the 
      PWM for motor control (0 to 4095).
**********************************************/
void Stepper_Set_Velocity(uint16_t value,  TIM_HandleTypeDef timer_handle)
{
    HAL_TIM_PWM_Stop(&timer_handle, TIM_CHANNEL_1);
    timer_handle.Init.Prescaler = value;
    if (HAL_TIM_Base_Init(&timer_handle) != HAL_OK)
    {
    Error_Handler();
    }
    HAL_TIM_PWM_Start(&timer_handle, TIM_CHANNEL_1);
}