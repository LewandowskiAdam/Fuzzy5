/*
    Load.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "Load.h"
#include "MemorySpecialAllocators.h"
#include "main.h"
#include "tim.h"

namespace LLA {
    void Load::init() {
        if (isInitialized == 0) {
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
            isInitialized = 1;
        }
    }

    void Load::setValue(LLA::Phase phase, int pwmPercentage) {
        uint8_t valueToSet = 0;
        if (pwmPercentage > 100) {
            valueToSet = 100;
        } else if (pwmPercentage < 0) {
            valueToSet = 0;
        } else {
            valueToSet = pwmPercentage;
        }
        switch (phase) {
            case LLA::Phase::PhaseA:
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, valueToSet);
                phaseAValue = valueToSet;
                break;
            case LLA::Phase::PhaseB:
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, valueToSet);
                phaseBValue = valueToSet;
                break;
            case LLA::Phase::PhaseC:
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, valueToSet);
                phaseCValue = valueToSet;
                break;
            default:
                break;
        }
    }

    int Load::getValue(LLA::Phase phase) {
        switch (phase) {
            case LLA::Phase::PhaseA:
                return phaseAValue;
                break;
            case LLA::Phase::PhaseB:
                return phaseBValue;
                break;
            case LLA::Phase::PhaseC:
                return phaseCValue;
                break;
            default:
                return 0;
                break;
        }
    }

    Load *Load::getInstance() {
        static Load loadInstance MEM_FORCE_D3RAM;
        return &loadInstance;

    }
}