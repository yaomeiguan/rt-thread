;-------------------------------------------------------------------------------
; sys_intvecs.asm
;
; (c) Texas Instruments 2009-2013, All rights reserved.
;

    .sect ".intvecs"
    .arm

;-------------------------------------------------------------------------------
; import reference for interrupt routines

    .ref _c_int00
    .ref _dabort
    .ref IRQ_Handler
    
    .def resetEntry

;-------------------------------------------------------------------------------
; interrupt vectors

resetEntry
        b   _c_int00
        b   turnon_VFP
svcEntry
        b   svcEntry
prefetchEntry
        b   prefetchEntry
        b   _dabort
reservedEntry
        b   reservedEntry
        b   IRQ_Handler
        ldr pc,[pc,#-0x1b0]

    .sect ".text"
turnon_VFP
        ; Enable FPV
        STMDB sp!,     {r0}
        fmrx  r0,      fpexc
        orr   r0,      r0,   #0x40000000
        fmxr  fpexc,   r0
        LDMIA sp!,     {r0}
        subs  pc,      lr,   #4
;-------------------------------------------------------------------------------
