#include "servo_mg90s.h"
#include "main.h"
#include "radar_config.h"

/*
 * Servo MG90S:
 * - Signal: PA1
 * - Timer: TIM5_CH2
 * - PWM 50Hz
 * - CCR2 = pulse width in microseconds
 */

extern TIM_HandleTypeDef htim5;

static uint16_t g_servo_current_angle = SERVO_CENTER_ANGLE_DEG;

void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
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

    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, pulse_us);
}

void Servo_SetAngle(uint16_t angle_deg)
{
    uint32_t pulse;

    if (angle_deg > SERVO_MAX_ANGLE_DEG)
    {
        angle_deg = SERVO_MAX_ANGLE_DEG;
    }

    /*
     * Linear mapping:
     * 0 deg   -> 500 us
     * 90 deg  -> 1500 us
     * 180 deg -> 2500 us
     */
    pulse = SERVO_MIN_PULSE_US;
    pulse += ((uint32_t)angle_deg *
             (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)) /
             SERVO_MAX_ANGLE_DEG;

    Servo_SetPulseUs((uint16_t)pulse);
    g_servo_current_angle = angle_deg;
}

uint16_t Servo_GetCurrentAngle(void)
{
    return g_servo_current_angle;
}
