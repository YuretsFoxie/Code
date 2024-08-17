.device  ATtiny13A
.include "tn13Adef.inc"

.equ idle				= 0b00000001
.equ firstPulse			= 0b00000010
.equ gap				= 0b00000100
.equ secondPulse		= 0b00001000

; the values (and Watchdog timeout) should be adjusted, they are aproximate

.equ firstPulseDelay	= 1
.equ gapDelay			= 1
.equ secondPulseDelay	= 8

.def Counter			= R18
.def State				= R19

;=====

.macro Load
ldi	 R16, @1
out	 @0, R16
.endm

.macro SetBits
in  R16, @0
ori R16, @1
out @0, R16
.endm

.macro ClearBits
in R16, @0
ldi R17, @1
com R17
and R16, R17
out @0, R16
.endm

.macro CaseState
cpi  State, @0
breq @1
.endm

.macro EnableExternalInterrupts
Load GIMSK, 1<<INT0
.endm

.macro DisableExternalInterrupts
Load GIMSK, 0<<INT0
.endm

.macro EnableSleepMode
SetBits MCUCR, 1<<SE
.endm

.macro DisableSleepMode
ClearBits MCUCR, 1<<SE
.endm

.macro SetLevelHigh
sbi PORTB, PB0
.endm

.macro SetLevelLow
cbi PORTB, PB0
.endm

.macro Loop
cpi  Counter, 0
breq @0
dec  Counter
.endm

.macro SetState
ldi State, @0
ldi Counter, @1
.endm

;=====

.macro InitStack
Load SPL, low(RAMEND)
.endm

.macro InitExternalInterrupt
Load MCUCR, 0<<ISC01 | 0<<ISC00				; The low level of INT0 generates an interrupt request
Load GIMSK, 1<<INT0							; Pin Change Interrupt Enable
.endm

.macro InitWatchdog
Load WDTCR, 1<<WDTIE | 1<<WDP2				; Watchdog interrupt is called about each 0.25 second
.endm

.macro InitPortB							; PB0 is LED output, PB1 is button input
Load DDRB,  0b00000001
Load PORTB, 0b00000010
.endm

.macro InitSleepMode
ClearBits MCUCR, 1<<SM1 | 1<<SM0			; 0 0 for Idle mode, 1 0 for Power-down mode
.endm

.macro ClearRegisters
Load GIFR, 0b11111111
ldi State, idle
clr Counter
.endm

.macro DisableComparator
Load ACSR, 1<<ACD | 0<<ACIE
.endm

.macro EnableInterrupts
sei
.endm

;=====

.macro Init
InitStack
InitExternalInterrupt
InitWatchdog
InitPortB
InitSleepMode
ClearRegisters
DisableComparator
EnableSleepMode
EnableInterrupts
.endm

.macro Run
Main:
sleep
rjmp Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0001 rjmp ButtonPress
.org 0x0008 rjmp WatchdogTimeout

;=====

Reset:
Init
Run

;=====

ButtonPress:
DisableSleepMode
DisableExternalInterrupts
SetState firstPulse, firstPulseDelay
reti

;=====

WatchdogTimeout:
CaseState firstPulse,	OnFirstPulse
CaseState gap,			OnGap
CaseState secondPulse,	OnSecondPulse
reti



OnFirstPulse:
SetLevelHigh
Loop FirstPulseIsOver
reti

FirstPulseIsOver:
SetLevelLow
SetState gap, gapDelay
reti



OnGap:
Loop GapIsOver
reti

GapIsOver:
SetLevelHigh
SetState secondPulse, secondPulseDelay
reti



OnSecondPulse:
Loop SecondPulseIsOver
reti

SecondPulseIsOver:
EnableSleepMode
SetLevelLow
EnableExternalInterrupts
SetState idle, 0
reti

;=====
