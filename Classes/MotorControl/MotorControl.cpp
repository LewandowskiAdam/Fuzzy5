/*
    MotorControl.cpp
    Created on: 24.02.23
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "MotorControl.h"
#include "MotorHighSide.h"
#include <cassert>
#include "SEGGER_RTT.h"

namespace MotorControl {
    MotorControl::MotorControl() {
    }

    void MotorControl::lockMotor() {
        lockState = true;
        turnOffAll();
    }

    void MotorControl::unlockMotor() {
        lockState = false;
    }

    void MotorControl::setMotorLowSideInstance(LLA::MotorLowSide *newInstance) {
        assert(newInstance != nullptr);
        motorLowSideInstance = newInstance;
    }

    void MotorControl::setSystemListener(NeuroFuzzy::SystemListener *newSystemListener) {
        assert(newSystemListener != nullptr);
        systemListener = newSystemListener;
    }

    void MotorControl::turnOffAll() {
        assert(motorLowSideInstance != nullptr);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseA);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseB);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseC);
        motorLowSideInstance->shutdown();
    }

}