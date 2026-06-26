#include "servo_mg90s.h"
#include "main.h"
#include "radar_config.h"

extern TIM_HandleTypeDef htim5;

static volatile uint16_t g_last_angle = SERVO_CENTER_ANGLE_DEG;
static volatile uint16_t g_last_pulse_us = SERVO_CENTER_PULSE_US;

void Servo_Init(void)
{
    /*
     * PA1 = TIM5_CH2
     * TIM5: PSC=89, ARR=19999 => 1 tick = 1us, period = 20ms
     */

    HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_2);

    __HAL_TIM_DISABLE(&htim5);

    __HAL_TIM_SET_PRESCALER(&htim5, 89);
    __HAL_TIM_SET_AUTORELOAD(&htim5, 19999);
    __HAL_TIM_SET_COUNTER(&htim5, 0);

    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, SERVO_CENTER_PULSE_US);

    HAL_TIM_GenerateEvent(&htim5, TIM_EVENTSOURCE_UPDATE);

    if (HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }

    Servo_SetAngle(SERVO_CENTER_ANGLE_DEG);
}

void Servo_SetPulseUs(uint16_t pulse_us)
{
    if (pulse_us < SERVO_MIN_PULSE_US)
    {
        pulse_us = SERVO_MIN_PULSE_US;
    }

    if (pulse_us > SERVO_MAX_PULSE_US)
    {
        pulse_us = SERVO_MAX_PULSE_US;
    }

    g_last_pulse_us = pulse_us;

    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, pulse_us);
}

void Servo_SetAngle(uint16_t angle_deg)
{
    uint32_t pulse_us;

    if (angle_deg > SERVO_MAX_ANGLE_DEG)
    {
        angle_deg = SERVO_MAX_ANGLE_DEG;
    }

    pulse_us = SERVO_MIN_PULSE_US +
               ((uint32_t)angle_deg *
               (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)) /
               SERVO_MAX_ANGLE_DEG;

    g_last_angle = angle_deg;

    Servo_SetPulseUs((uint16_t)pulse_us);
}

uint16_t Servo_GetLastAngle(void)
{
    return g_last_angle;
}

uint16_t Servo_GetLastPulseUs(void)
{
    return g_last_pulse_us;
}
