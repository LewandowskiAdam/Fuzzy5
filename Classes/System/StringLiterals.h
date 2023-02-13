/*
    StringLiterals.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_STRINGLITERALS_H
#define NEUROFUZZYS22_STRINGLITERALS_H

namespace StringLiterals {
    //messages
    const char InitializationStart[] = "Starting system initialization...!\n";
    const char InitializationComplete[] = "System initialized!\n";
    const char EmptyMessage[] = "Message empty!\n";
    const char TooLongMessage[] = "Message too long!\n";
    const char UartTxQueueName[] = "Uart tx queue";
    const char UartRxQueueName[] = "Uart rx queue";
    const char LoadQueueName[] = "Load queue";
    const char Separator[] = " ";
    const char MonitoringStartSeparator[] = "-------------------------------Tasks--------------------------------\n";
    const char MonitoringSeparator[] = "--------------------------------------------------------------------\n";

    //control literals
    const char Set[] = "-set";
    const char Get[] = "-get";
    const char Load[] = "-load ";

    //return messages
    const char LoadSetMessage[] = "Report: -load -set ";
    const char LoadGetMessage[] = "Report: -load -get ";
    const char LoadSetNotProperlyFormatted[] = "Report: Load control string not properly formatted";

}

#endif