/*
    MonitorTask.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MONITORTASK_H
#define NEUROFUZZYS22_MONITORTASK_H

#include "SystemListener.h"

namespace Tasks {
    class MonitorTask {
    private:
    protected:
        NeuroFuzzy::SystemListener* systemListener = nullptr;
    public:
        MonitorTask(){}
        void task();
        void setSystemListener(NeuroFuzzy::SystemListener* newSystemListener);
    };
}
#endif