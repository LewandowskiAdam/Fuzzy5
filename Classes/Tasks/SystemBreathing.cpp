/*
    Default.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "SystemBreathing.h"
#include "Led.h"
#include "cmsis_os.h"

namespace Tasks {
    SystemBreathing::SystemBreathing() {}
    void SystemBreathing::task() {
        while(1){
            osDelay(ledBlinkingPeriod);
            LLA::Led::toggleLed(LLA::LedRed0());
        }
    }
}