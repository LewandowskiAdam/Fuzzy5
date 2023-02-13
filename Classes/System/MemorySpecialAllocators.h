/*
    MemorySpecialAllocators.h
    Author: Adam Lewandowski
    @startuml
    @enduml
*/

#ifndef NEUROFUZZYS22_MEMORYSPECIALALLOCATORS_H
#define NEUROFUZZYS22_MEMORYSPECIALALLOCATORS_H

#define MEM_FORCE_D1RAM __attribute__ ((section(".ram1Data")));
#define MEM_FORCE_D2RAM __attribute__ ((section(".ram2Data")));
#define MEM_FORCE_D3RAM __attribute__ ((section(".ram3Data")));
#define MEM_FORCE_DTCMRAM __attribute__ ((section(".dtcmram")));
#define MEM_FORCE_ITCMRAM __attribute__ ((section(".itcmram")));

#endif