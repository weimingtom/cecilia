#pragma once

extern DL_IMPORT(int) PyOS_InterruptOccurred(void);
extern DL_IMPORT(void) PyOS_InitInterrupts(void);
DL_IMPORT(void) PyOS_AfterFork(void);
