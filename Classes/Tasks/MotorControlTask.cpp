/*
    MotorControlTask.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "cmsis_os.h"
#include "MotorControlTask.h"
#include "Led.h"

namespace Tasks {
    MotorControlTask::MotorControlTask() {
    }

    void MotorControlTask::task() {
        while(1){
            LLA::Led::on(LLA::LedBlue());
            osDelay(100);
            LLA::Led::off(LLA::LedBlue());
            osDelay(100);
        }
    }
}