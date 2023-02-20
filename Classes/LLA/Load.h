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
        uint8_t isInitialized = 0;
        uint8_t phaseAValue = 0;
        uint8_t phaseBValue = 0;
        uint8_t phaseCValue = 0;

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