/*
    Load.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_LOAD_H
#define NEUROFUZZYS22_LOAD_H

#include "Phase.h"
#include <cstdint>

namespace LLA {
    class Load {
    private:
    protected:
        uint8_t isInitialized;
        uint8_t phaseAValue;
        uint8_t phaseBValue;
        uint8_t phaseCValue;

        Load() {};

        Load(const Load &) {};
    public:

        void setValue(LLA::Phase phase, int pwmPercentage);

        int getValue(LLA::Phase phase);

        void init();

        static Load *getInstance();
    };
}
#endif