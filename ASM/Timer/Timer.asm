.device  ATmega16A
.include "m16Adef.inc"

;=====

.macro Load
ldi	 R16, @1
out	 @0, R16
.endm

.macro InvertBits
ldi  R17, @1
in   R16, @0
eor  R16, R17
out  @0, R16
.endm

;=====

.macro InitStack
Load SPL, low(RAMEND)
Load SPH, high(RAMEND)
.endm

.macro InitTimer
Load ASSR, 1<<AS2						; use the external clock oscillator as the clock source for the Timer/Counter2 (async mode)
Load TCCR2, 1<<CS22 | 0<<CS21 | 1<<CS20	; 1 s, prescaler is 128 (32768 Hz / 256 = 128 Hz)
Load TIMSK, 1<<TOIE2					; Timer/Counter2 Overflow Interrupt Enable
Load TIFR, 0b11111111					; clear all interrupt flags
.endm

.macro InitPortB
Load DDRB,  0b11111111
Load PORTB, 0b00000001
.endm

.macro DisableUnused
Load ACSR, 0<<ACD | 0<<ACIE
.endm

.macro SetFlags
sei
.endm

;=====

.macro Init
InitStack
InitTimer
InitPortB
DisableUnused
SetFlags
.endm

.macro Run
Main:
rjmp Main
.endm

;=====

.org 0x0000 jmp Reset
.org 0x0008 jmp OnTimer2Overflow

;=====

Reset:
Init
Run

;=====

OnTimer2Overflow:
InvertBits PORTB, 0b00000011
reti

;=====
