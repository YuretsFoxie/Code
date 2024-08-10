.device  ATtiny13A
.include "tn13Adef.inc"

.equ idle				= 0b00000001
.equ firstPress			= 0b00000010
.equ firstPressFailed	= 0b00000100
.equ gap				= 0b00001000
.equ longPress			= 0b00010000

.equ firstPressDelay	= 31		; 10 - 320 milliseconds
.equ gapDelay			= 62		; 2 seconds
.equ longPressDelay		= 94		; 3 seconds

.def Flags				= R17
.def Counter			= R18
.def State				= R19

;=====

.macro Load
ldi	 R16, @1
out	 @0, R16
.endm

.macro CaseState
cpi State, @0
breq @1
.endm

;=====

.macro InitStack
Load SPL, low(RAMEND)
.endm

.macro InitWatchdog
Load WDTCR, 1<<WDTIE | 1<<WDP0		; Watchdog interrupt is called each 32 milliseconds (5000 / 32 = 156.25)
.endm

.macro InitPortB					; PB0 is LED output, PB1 is button input
Load DDRB,  0b00000001
Load PORTB, 0b00000010
.endm

.macro ClearRegisters
clr Flags
clr Counter
ldi State, idle
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
InitWatchdog
InitPortB
ClearRegisters
DisableComparator
EnableInterrupts
.endm

.macro Run
Main:
rjmp Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0008 rjmp WatchdogTimeout

;=====

Reset:
Init
Run

;=====

; Stage 1: toggle the LED, if the press was shorter, than firstPressDelay

; if state == idle AND level == 0, start the counter, state = firstPress
; if (state == firstPress OR state == firstPressFailed) counter != 0, decrement the counter

; if state == firstPress AND counter == 0 AND level == 0, start the counter, state = firstPressFailed
; if state == firstPress AND counter == 0 AND level == 1, state = idle, toggle LED
; if state == firstPressFailed AND counter == 0 AND level == 0, start the counter, state = firstPressFailed
; if state == firstPressFailed AND counter == 0 AND level == 1, state = idle

WatchdogTimeout:
CaseState idle, OnIdle
CaseState firstPress, OnFirstPress
CaseState firstPressFailed, OnFirstPressFailed
reti

;-----

OnIdle:
sbis PINB, PB1
rjmp IdleLow
reti

IdleLow:
ldi Counter, firstPressDelay
ldi State, firstPress
reti

;-----

OnFirstPress:
cpi  Counter, 1
brsh Decrement

sbis PINB, PB1
rjmp FirstPressLow

ldi State, idle
sbi PINB, PB0
reti

FirstPressLow:
ldi Counter, firstPressDelay
ldi State, firstPressFailed
reti

;-----

OnFirstPressFailed:
cpi  Counter, 1
brsh Decrement

sbis PINB, PB1
rjmp FirstPressFailedLow

ldi State, idle
reti

FirstPressFailedLow:
ldi Counter, firstPressDelay
ldi State, firstPressFailed
reti

;-----

Decrement:
dec Counter
reti

;=====
