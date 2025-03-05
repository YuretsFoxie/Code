.device ATmega8
.include "m8def.inc"

.macro Load
ldi R16, @1
out @0, R16
.endm

.macro InvertBits
ldi  R17, @1
in   R16, @0
eor  R16, R17
out  @0, R16
.endm

.macro InitStack
Load SPL, Low(RAMEND)
Load SPH, High(RAMEND)
.endm

.macro InitPorts
sbi DDRB, PB1
sbi DDRB, PB2
sbi PORTB, PB1
cbi PORTB, PB2
.endm

.macro InitTimer
Load TCCR0, (1<<CS02) | (1<<CS00)	; Prescaler 1024
Load TIMSK, (1<<TOIE0)				; Enable overflow interrupt
.endm

.macro DisableUnused
Load ACSR, 1<<ACD
.endm

.macro EnableInterrupts
sei
.endm

.macro Init
InitStack
InitPorts
InitTimer
DisableUnused
EnableInterrupts
.endm

.macro Run
Main: 
rjmp Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0009	rjmp OnTimer0Overflow

;=====

Reset:
Init
Run

OnTimer0Overflow:
InvertBits PORTB, 0b00000110
Load TCNT0, 246					; preloaded value for 50 Hz
reti

;=====
