/*
    MonitorTask.cpp
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#include "MonitorTask.h"
#include "Configuration.h"
#include "cmsis_os.h"
#include <cassert>

namespace Tasks {
    void MonitorTask::setSystemListener(NeuroFuzzy::SystemListener *newSystemListener) {
        assert(newSystemListener != nullptr);
        systemListener = newSystemListener;
    }

    void MonitorTask::task() {
        static char data[MONITORING_TASK_BUFFER_DATA_SIZE] RTOS_MEMORY_ALLOCATION;;
        while (1) {
            #if MONITORING_BLINK_ACTIVITY
                osDelay(MONITORING_TASK_INTERVAL_MS-MONITORING_TASK_LED_ON_TIME_MS);
                LLA::Led::on(LLA::LedBlue());
                osDelay(MONITORING_TASK_LED_ON_TIME_MS);
                LLA::Led::off(LLA::LedBlue());
                vTaskGetRunTimeStats(data);
            #else
                osDelay(MONITORING_TASK_INTERVAL_MS);
            #endif
            #if MONITORING_USE_RTT
                vTaskGetRunTimeStats(data);
                SEGGER_RTT_printf(0, StringLiterals::MonitoringStartSeparator);
                SEGGER_RTT_printf(0, data);
                SEGGER_RTT_printf(0, StringLiterals::MonitoringSeparator);
            #endif
            #if MONITORING_USE_UART
                vTaskGetRunTimeStats(data);
                systemListener->uartSend(std::string(StringLiterals::MonitoringStartSeparator));
                systemListener->uartSend(std::string(data));
                systemListener->uartSend(std::string(StringLiterals::MonitoringSeparator));
            #endif
        }
    }
}