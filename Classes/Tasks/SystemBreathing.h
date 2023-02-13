/*
    SystemBreathing.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_SYSTEMBREATHING_H
#define NEUROFUZZYS22_SYSTEMBREATHING_H

#include "Configuration.h"

namespace Tasks {
    class SystemBreathing {
    private:
    protected:
        const int ledBlinkingPeriod = DEFAULT_TASK_LED_INTERVAL_MS;
    public:
        SystemBreathing();

        void task();
    };
}
#endif