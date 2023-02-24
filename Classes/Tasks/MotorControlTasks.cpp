/*
    MotorControlTasks.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "cmsis_os.h"
#include "MotorControlTasks.h"
#include "Led.h"
#include "Adc.h"
#include <cassert>

#include "SEGGER_RTT.h"

namespace Tasks {
    MotorControlTasks::MotorControlTasks() {
    }

    void MotorControlTasks::task() {
        SEGGER_RTT_printf(0, "MotorControlTask start!\n");
        adcInstance = LLA::Adc::getInstance();
        while (1) {
            printAdcValuesCSV();
            osDelay(1);
            /*
            LLA::Led::on(LLA::LedBlue());
            osDelay(100);
            LLA::Led::off(LLA::LedBlue());
            osDelay(100);*/
        }
    }

    void MotorControlTasks::printAdcValues() {
        assert(adcInstance != nullptr);
        SEGGER_RTT_printf(0, "ADC values --- PhaseA: %d PhaseB: %d PhaseC: %d Virtual ZERO: %d\n",
                          adcInstance->getValue(LLA::Phase::PhaseA), adcInstance->getValue(LLA::Phase::PhaseB),
                          adcInstance->getValue(LLA::Phase::PhaseC), adcInstance->getValue(LLA::Phase::Zero));
    }
    void MotorControlTasks::printAdcValuesCSV() {
        assert(adcInstance != nullptr);
        SEGGER_RTT_printf(0, "%d,%d,%d,%d;\n",
                          adcInstance->getValue(LLA::Phase::PhaseA), adcInstance->getValue(LLA::Phase::PhaseB),
                          adcInstance->getValue(LLA::Phase::PhaseC), adcInstance->getValue(LLA::Phase::Zero));
    }
}