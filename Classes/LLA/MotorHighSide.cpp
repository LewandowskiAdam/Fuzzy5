/*
    MotorHighSize.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "MotorHighSide.h"
#include "main.h"

namespace LLA {
    void MotorHighSide::switchOff(LLA::Phase mosPhase) {
        switch (mosPhase) {
            case LLA::Phase::PhaseA:
                HAL_GPIO_WritePin(MOSH0_GPIO_Port, MOSH0_Pin, GPIO_PIN_RESET);
                break;
            case LLA::Phase::PhaseB:
                HAL_GPIO_WritePin(MOSH1_GPIO_Port, MOSH1_Pin, GPIO_PIN_RESET);
                break;
            case LLA::Phase::PhaseC:
                HAL_GPIO_WritePin(MOSH2_GPIO_Port, MOSH2_Pin, GPIO_PIN_RESET);
                break;
            default:
                break;
        }
    }

    void MotorHighSide::switchOn(LLA::Phase mosPhase) {
        switch (mosPhase) {
            case LLA::Phase::PhaseA:
                HAL_GPIO_WritePin(MOSH0_GPIO_Port, MOSH0_Pin, GPIO_PIN_SET);
                break;
            case LLA::Phase::PhaseB:
                HAL_GPIO_WritePin(MOSH1_GPIO_Port, MOSH1_Pin, GPIO_PIN_SET);
                break;
            case LLA::Phase::PhaseC:
                HAL_GPIO_WritePin(MOSH2_GPIO_Port, MOSH2_Pin, GPIO_PIN_SET);
                break;
            default:
                break;
        }
    }

    void MotorHighSide::toggle(LLA::Phase mosPhase) {
        switch (mosPhase) {
            case LLA::Phase::PhaseA:
                HAL_GPIO_TogglePin(MOSH0_GPIO_Port, MOSH0_Pin);
                break;
            case LLA::Phase::PhaseB:
                HAL_GPIO_TogglePin(MOSH1_GPIO_Port, MOSH1_Pin);
                break;
            case LLA::Phase::PhaseC:
                HAL_GPIO_TogglePin(MOSH2_GPIO_Port, MOSH2_Pin);
                break;
            default:
                break;
        }
    }
}