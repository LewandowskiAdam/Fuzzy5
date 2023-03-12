/*
    MotorControlTasks.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "cmsis_os.h"
#include "MotorControlTasks.h"
#include "Led.h"
#include "MotorHighSide.h"
#include <cassert>

#include "SEGGER_RTT.h"

namespace Tasks {
/*----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *--------------------------------------------------------------------------------------------------------------------*/
    void MotorControlTasks::task() {
        while (1) {
            osDelay(10);
            //SEGGER_RTT_printf(0, "ADC freq: %d\n", count);
            printADCValuesCSV();
            count = 0;
        }
    }

/*----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *--------------------------------------------------------------------------------------------------------------------*/

    MotorControlTasks::MotorControlTasks() {
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

    void MotorControlTasks::setAdcInstance(LLA::Adc *newInstance) {
        assert(newInstance!= nullptr);
        adcInstance = newInstance;
        motorControl.setAdcInstance(newInstance);
    }

    void MotorControlTasks::printADCValues() {
        SEGGER_RTT_printf(0, "PhaseA: %d, PhaseB: %d, PhaseC: %d, ZERO: %d\n",
                          adcInstance->getValue(LLA::Phase::PhaseA),
                          adcInstance->getValue(LLA::Phase::PhaseB),
                          adcInstance->getValue(LLA::Phase::PhaseC),
                          adcInstance->getValue(LLA::Phase::Zero));
    }
    void MotorControlTasks::printADCValuesCSV() {
        SEGGER_RTT_printf(0, "%d, %d, %d, %d;\n",
                          adcInstance->getValue(LLA::Phase::PhaseA),
                          adcInstance->getValue(LLA::Phase::PhaseB),
                          adcInstance->getValue(LLA::Phase::PhaseC),
                          adcInstance->getValue(LLA::Phase::Zero));
    }
/*----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *--------------------------------------------------------------------------------------------------------------------*/

    void MotorControlTasks::buttonISR(int buttonPressed) {
        //lock motor if it is running (always)
        if (!motorControl.getLockState()) {
            //locking motor on any button press for safety reasons
            motorControl.lockMotor();
            systemListener->uartSendFromISR("Motor locked\n");
            LLA::Led::off(LLA::LedBlue());
        } else {
            //unlock only when motor previously locked, and pressed specific button
            if (buttonPressed == 1) {
                motorControl.unlockMotor();
                systemListener->uartSendFromISR("Motor unlocked\n");
                LLA::Led::on(LLA::LedBlue());
                LLA::MotorHighSide::switchOn(LLA::Phase::PhaseA);
                motorLowSideInstance->setValue(LLA::Phase::PhaseB, 10);
            }
        }
    }

/*----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *--------------------------------------------------------------------------------------------------------------------*/

    void MotorControlTasks::adcISR() {
        motorControl.adcISR();
        count++;
    }

/*----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *----------------------------------------------------------------------------------------------------------------------
 *--------------------------------------------------------------------------------------------------------------------*/
}