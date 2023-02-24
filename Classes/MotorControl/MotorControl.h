/*
    MotorControl.h
    Created on: 24.02.23
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTORCONTROL_H
#define NEUROFUZZYS22_MOTORCONTROL_H

#include "MotorHighSide.h"
#include "MotorLowSide.h"

namespace MotorControl {
    class MotorControl {
    private:
    protected:
        //if set to one all outputs are blocked in reset state (to change state motor will need to be first unlocked)
        bool lockState = true;

    public:
        MotorControl();
    };
}
#endif