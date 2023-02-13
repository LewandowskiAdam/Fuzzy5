/*
    System.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "System.h"
#include "MotorHighSide.h"
#include "StringLiterals.h"
#include "UartString.h"

Tasks::SystemBreathing NeuroFuzzy::System::defaultTask;
Tasks::UartTasks NeuroFuzzy::System::uartTasks;
Tasks::MonitorTask NeuroFuzzy::System::monitorTask;
Tasks::LoadTask NeuroFuzzy::System::loadTask;

namespace NeuroFuzzy {
    System::System() {
    }

    void System::init() {
        uartSend(StringLiterals::InitializationStart);
        adc = LLA::Adc::getInstance();
        adc->init();
        load = LLA::Load::getInstance();
        load->init();
        load->setValue(LLA::Phase::PhaseA, 0);
        load->setValue(LLA::Phase::PhaseB, 0);
        load->setValue(LLA::Phase::PhaseC, 0);
        motorLowSide = LLA::MotorLowSide::getInstance();
        motorLowSide->init();
        uart = LLA::Uart::getInstance();
        uart->init();
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseA);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseB);
        LLA::MotorHighSide::switchOff(LLA::Phase::PhaseC);
        uartTasks.setUartClassInstance(uart);
        uartTasks.setSystemControlListener(this);
        uart->setListener(this);
        uartSend(StringLiterals::InitializationComplete);
        loadTask.setSystemListener(this);
    }

    //interfaces methods
    void System::dataReceivedFromISR(LLA::UartString uartString) {
        uartTasks.addToRxQueueFromISR(uartString);
    }

    void System::uartSend(std::string message) {
        uartTasks.addToTxQueue(message);
    }

    void System::loadControlListener(std::string message) {
        loadTask.addToQueue(LLA::UartString(message));
    }
}
