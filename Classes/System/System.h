/*
    System.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_SYSTEM_H
#define NEUROFUZZYS22_SYSTEM_H

#include "Adc.h"
#include "Load.h"
#include "MotorLowSide.h"
#include "SystemBreathing.h"
#include "UartTasks.h"
#include "SystemListener.h"
#include "MonitorTask.h"
#include "LoadTask.h"
#include "UartString.h"
#include "MotorControlTasks.h"

namespace NeuroFuzzy {

class System : public LLA::UartListener, public SystemListener{
    private:
    protected:
        LLA::Adc* adc;
        LLA::Load* load;
        LLA::MotorLowSide* motorLowSide;
        LLA::Uart* uart;

        void buttonNotPresentInterruptRoutine();
        void button1InterruptRoutine();
        void button2InterruptRoutine();

    public:
        static Tasks::SystemBreathing defaultTask RTOS_MEMORY_ALLOCATION;
        static Tasks::UartTasks uartTasks RTOS_MEMORY_ALLOCATION;
        static Tasks::MonitorTask monitorTask RTOS_MEMORY_ALLOCATION;
        static Tasks::LoadTask loadTask RTOS_MEMORY_ALLOCATION;
        static Tasks::MotorControlTasks motorControlTask RTOS_MEMORY_ALLOCATION;
        System();
        void init();
        void gpioInterruptRoutine(uint16_t GpioPin);

        //methods overrides
        void dataReceivedFromISR(LLA::UartString uartString) override;
        void uartSend(std::string message) override;
        void loadControlListener(std::string message) override;
    };
}
#endif
