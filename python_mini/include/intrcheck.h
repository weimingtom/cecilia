//20180318
#pragma once

extern DL_IMPORT(int) PyOS_InterruptOccurred();
extern DL_IMPORT(void) PyOS_InitInterrupts();
DL_IMPORT(void) PyOS_AfterFork();
