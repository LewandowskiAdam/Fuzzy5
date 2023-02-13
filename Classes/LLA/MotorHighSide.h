/*
    MotorHighSize.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MOTOHIGHSIZE_H
#define NEUROFUZZYS22_MOTOHIGHSIZE_H

#include "Phase.h"

namespace LLA {
    class MotorHighSide {
    private:
    protected:
        MotorHighSide() {};
    public:
        static void switchOn(LLA::Phase phase);

        static void switchOff(LLA::Phase phase);

        static void toggle(LLA::Phase phase);
    };
}
#endif
