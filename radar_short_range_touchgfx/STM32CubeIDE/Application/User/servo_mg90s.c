#include "servo_mg90s.h"
#include "main.h"
#include "radar_config.h"

extern TIM_HandleTypeDef htim3;

static volatile uint16_t g_last_angle = SERVO_CENTER_ANGLE_DEG;
static volatile uint16_t g_last_pulse_us = SERVO_CENTER_PULSE_US;

void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*
     * Servo signal = PB4 = TIM3_CH1
     * TIM3: PSC=89, ARR=19999 => 1 tick = 1us, period = 20ms
     */

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);

    __HAL_TIM_DISABLE(&htim3);

    __HAL_TIM_SET_PRESCALER(&htim3, 89);
    __HAL_TIM_SET_AUTORELOAD(&htim3, 19999);
    __HAL_TIM_SET_COUNTER(&htim3, 0);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, SERVO_CENTER_PULSE_US);

    HAL_TIM_GenerateEvent(&htim3, TIM_EVENTSOURCE_UPDATE);

    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK)
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

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse_us);
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
