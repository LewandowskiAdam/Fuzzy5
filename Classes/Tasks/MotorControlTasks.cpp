/*
    MotorControlTasks.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "cmsis_os.h"
#include "MotorControlTasks.h"
#include <cassert>

#include "SEGGER_RTT.h"

namespace Tasks {
    MotorControlTasks::MotorControlTasks() {
    }

    void MotorControlTasks::task() {
        adcInstance = LLA::Adc::getInstance();
        while (1) {
            osDelay(1000);
            SEGGER_RTT_printf(0, "ADC freq: %d\n", count);
            count=0;
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

    void MotorControlTasks::setSystemListenerInstance(NeuroFuzzy::SystemListener *newSystemListener) {
        assert(newSystemListener != nullptr);
        systemListener = newSystemListener;
        motorControl.setSystemListener(newSystemListener);
    }

    void MotorControlTasks::setMotorLowSide(LLA::MotorLowSide *newInstance) {
        assert(newInstance != nullptr);
        motorLowSideInstance = newInstance;
        motorControl.setMotorLowSideInstance(newInstance);
    }

    void MotorControlTasks::buttonISR(int buttonPressed) {
        if (!motorLocked) {
            //locking motor on any button press for safety reasons
            motorControl.lockMotor();
            motorLocked = true;
            systemListener->uartSendFromISR("Motor locked\n");
        } else {
            //unlock only when motor previously locked, and pressed specific button
            if(buttonPressed==1){
                motorControl.unlockMotor();
                motorLocked=false;
                systemListener->uartSendFromISR("Motor unlocked\n");
            }
        }
    }

    void MotorControlTasks::adcISR() {
        count++;
    }
}