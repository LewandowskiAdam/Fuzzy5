/*
    MotorControl.h
    Created on: 24.02.23
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTORCONTROL_H
#define NEUROFUZZYS22_MOTORCONTROL_H

#include "MotorLowSide.h"
#include "SystemListener.h"

namespace MotorControl {
    class MotorControl {
    private:
    protected:
        NeuroFuzzy::SystemListener* systemListener = nullptr;
        LLA::MotorLowSide* motorLowSideInstance = nullptr;

        //if set to one all outputs are blocked in reset state (to change state motor will need to be first unlocked)
        bool lockState = true;
        void turnOffAll();
    public:
        MotorControl();
        void lockMotor();
        void unlockMotor();
        void setMotorLowSideInstance(LLA::MotorLowSide* newInstance);
        void setSystemListener(NeuroFuzzy::SystemListener* newSystemListener);
    };
}

#endif