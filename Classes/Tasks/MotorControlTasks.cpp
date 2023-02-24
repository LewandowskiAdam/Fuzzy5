/*
    MotorControlTasks.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "cmsis_os.h"
#include "MotorControlTasks.h"
#include "SEGGER_RTT.h"
#include "Led.h"
#include "Adc.h"

namespace Tasks {
    MotorControlTasks::MotorControlTasks() {
    }

    void MotorControlTasks::task() {
        SEGGER_RTT_printf(0, "MotorControlTask start!\n");
        adcInstance = LLA::Adc::getInstance();
        while(1){
            LLA::Led::on(LLA::LedBlue());
            osDelay(100);
            LLA::Led::off(LLA::LedBlue());
            osDelay(100);
        }
    }

    void MotorControlTasks::printAdcValues() {

    }
}