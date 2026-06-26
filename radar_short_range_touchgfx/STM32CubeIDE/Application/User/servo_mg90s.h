#ifndef SERVO_MG90S_H
#define SERVO_MG90S_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void Servo_Init(void);
void Servo_SetPulseUs(uint16_t pulse_us);
void Servo_SetAngle(uint16_t angle_deg);

uint16_t Servo_GetLastAngle(void);
uint16_t Servo_GetLastPulseUs(void);

#ifdef __cplusplus
}
#endif

#endif /* SERVO_MG90S_H */
