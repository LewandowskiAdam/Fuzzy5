/*
    MotorLowSide.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include <cstdint>

#include "MotorLowSide.h"
#include "tim.h"
#include "stm32h7xx_ll_gpio.h"
#include "MemorySpecialAllocators.h"

namespace LLA {
    void MotorLowSide::init() {
        if (isInitialized == 0) {
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
            shutdown();
            isInitialized = 1;
        }
    }

    void MotorLowSide::shutdownAPhase() {
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
        asm("NOP");
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6);
    }

    void MotorLowSide::shutdownBPhase() {
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
        asm("NOP");
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7);
    }

    void MotorLowSide::shutdownCPhase() {
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
        LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
        asm("NOP");
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
    }

    void MotorLowSide::shutdown() {
        shutdownAPhase();
        shutdownBPhase();
        shutdownCPhase();
    }

    void MotorLowSide::enableAPhase() {
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
    }

    void MotorLowSide::enableBPhase() {
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
    }

    void MotorLowSide::enableCPhase() {
        LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
    }

    int MotorLowSide::getValue(LLA::Phase phase) {
        switch (phase) {
            case LLA::Phase::PhaseA:
                return phaseAValue;
            case LLA::Phase::PhaseB:
                return phaseBValue;
            case LLA::Phase::PhaseC:
                return phaseCValue;
            default:
                return 0;
        }
    }

    void MotorLowSide::setValue(LLA::Phase phase, int pwmPercentage) {
        uint8_t valueToSet = 0;
        if (pwmPercentage < 0) {
            valueToSet = 0;
        } else if (pwmPercentage > 100) {
            valueToSet = 100;
        } else {
            valueToSet = pwmPercentage;
        }

        switch (phase) {
            case LLA::Phase::PhaseA:
                if (valueToSet == 0) {
                    shutdownAPhase();
                            __HAL_TIM_SetCounter(&htim3, 0);
                } else {
                    enableAPhase();
                    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, valueToSet);
                }
                break;
            case LLA::Phase::PhaseB:
                if (valueToSet == 0) {
                    shutdownBPhase();
                            __HAL_TIM_SetCounter(&htim3, 0);
                } else {
                    enableBPhase();
                    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, valueToSet);
                }
                break;
            case LLA::Phase::PhaseC:
                if (valueToSet == 0) {
                    shutdownCPhase();
                            __HAL_TIM_SetCounter(&htim3, 0);
                } else {
                    enableCPhase();
                    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, valueToSet);
                }
                break;
            default:
                break;
        }
    }

    MotorLowSide *MotorLowSide::getInstance() {
        static MotorLowSide motorLowSideInstance MEM_FORCE_D3RAM;
        return &motorLowSideInstance;
    }
}