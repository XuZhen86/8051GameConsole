#ifndef FAR_H_
#define FAR_H_

// Init off-chip SRAM
void Far_init();

// Print statistical data
void Far_memInfo();

// Print chain of memory blocks
void Far_dumpMemBlock();

// Benchmark allocation and deallocation
void Far_speedTest();

#endif
