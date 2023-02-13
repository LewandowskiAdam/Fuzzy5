/*
    Led.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "Led.h"
#include "main.h"

namespace LLA {
    void Led::toggleLed(LLA::LedRed0 led) {
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
    }

    void Led::toggleLed(LLA::LedBlue led) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    }

    void Led::toggleLed(LLA::LedRed1 led) {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    }

    void Led::on(LLA::LedRed0) {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PinState::GPIO_PIN_RESET);
    }

    void Led::on(LLA::LedBlue) {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PinState::GPIO_PIN_RESET);
    }

    void Led::on(LLA::LedRed1) {
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PinState::GPIO_PIN_RESET);
    }

    void Led::off(LLA::LedRed0) {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PinState::GPIO_PIN_SET);
    }

    void Led::off(LLA::LedBlue) {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PinState::GPIO_PIN_SET);
    }

    void Led::off(LLA::LedRed1) {
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PinState::GPIO_PIN_SET);
    }

}