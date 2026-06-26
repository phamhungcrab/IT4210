#include "servo_mg90s.h"
#include "main.h"
#include "radar_config.h"

#include "FreeRTOS.h"
#include "task.h"

/*
 * SOFTWARE PWM SERVO VERSION
 *
 * Không dùng CubeMX.
 * Không dùng TIM3/TIM5.
 *
 * Servo signal = PB4 GPIO output.
 *
 * Lý do:
 * - PA1/TIM5_CH2 không ra mức HIGH thật trên board của ông.
 * - PB4 đã test LED HIGH/LOW OK.
 * - Tránh generate CubeMX phá TouchGFX/LCD.
 */

#define SERVO_GPIO_PORT   GPIOB
#define SERVO_GPIO_PIN    GPIO_PIN_4

static volatile uint16_t g_last_angle = SERVO_CENTER_ANGLE_DEG;
static volatile uint16_t g_last_pulse_us = SERVO_CENTER_PULSE_US;

static TaskHandle_t g_servo_pwm_task_handle = NULL;
static uint32_t g_cycles_per_us = 0;

static void Servo_DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;

    g_cycles_per_us = HAL_RCC_GetHCLKFreq() / 1000000U;
}

static void Servo_DelayUs(uint32_t us)
{
    uint32_t start;
    uint32_t ticks;

    if (g_cycles_per_us == 0U)
    {
        g_cycles_per_us = HAL_RCC_GetHCLKFreq() / 1000000U;
    }

    start = DWT->CYCCNT;
    ticks = us * g_cycles_per_us;

    while ((DWT->CYCCNT - start) < ticks)
    {
        /* busy wait only for 1ms - 2ms servo pulse */
    }
}

static void Servo_PWM_Task(void *argument)
{
    (void)argument;

    for (;;)
    {
        uint16_t pulse_us = g_last_pulse_us;
        uint32_t low_us;

        if (pulse_us < SERVO_MIN_PULSE_US)
        {
            pulse_us = SERVO_MIN_PULSE_US;
        }

        if (pulse_us > SERVO_MAX_PULSE_US)
        {
            pulse_us = SERVO_MAX_PULSE_US;
        }

        /*
         * Servo frame: 20ms.
         * HIGH: 1000-2000us.
         * LOW : phần còn lại.
         */
        HAL_GPIO_WritePin(SERVO_GPIO_PORT, SERVO_GPIO_PIN, GPIO_PIN_SET);
        Servo_DelayUs(pulse_us);

        HAL_GPIO_WritePin(SERVO_GPIO_PORT, SERVO_GPIO_PIN, GPIO_PIN_RESET);

        low_us = 20000U - pulse_us;

        /*
         * Nhường CPU phần lớn thời gian để TouchGFX không đơ.
         */
        vTaskDelay(pdMS_TO_TICKS(low_us / 1000U));

        /*
         * Bù phần lẻ micro giây.
         */
        Servo_DelayUs(low_us % 1000U);
    }
}

void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    Servo_DWT_Init();

    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*
     * PB4 = GPIO output push-pull.
     * Không cần CubeMX cấu hình PB4.
     */
    HAL_GPIO_DeInit(SERVO_GPIO_PORT, SERVO_GPIO_PIN);

    GPIO_InitStruct.Pin = SERVO_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SERVO_GPIO_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(SERVO_GPIO_PORT, SERVO_GPIO_PIN, GPIO_PIN_RESET);

    g_last_angle = SERVO_CENTER_ANGLE_DEG;
    g_last_pulse_us = SERVO_CENTER_PULSE_US;

    if (g_servo_pwm_task_handle == NULL)
    {
        BaseType_t ok;

        ok = xTaskCreate(
            Servo_PWM_Task,
            "servo_pwm",
            256,
            NULL,
            tskIDLE_PRIORITY + 1,
            &g_servo_pwm_task_handle
        );

        if (ok != pdPASS)
        {
            Error_Handler();
        }
    }
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
