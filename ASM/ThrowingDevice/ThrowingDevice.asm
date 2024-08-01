.device  ATtiny13A
.include "tn13Adef.inc"

.equ F_SYSTEM	= 4800000
.equ PRESCALER	= 1024
.equ F_MAIN		= 50

; TODO: adjust the values

.equ F_MIN		= 400	; 500
.equ F_MID		= 600	; 750
.equ F_MAX		= 1600	; 1000

.equ MATCH_MAIN	= F_SYSTEM / (F_MAIN * PRESCALER) - 1
.equ MATCH_MIN	= F_SYSTEM / (F_MIN * PRESCALER) - 1
.equ MATCH_MID	= F_SYSTEM / (F_MID * PRESCALER) - 1
.equ MATCH_MAX	= F_SYSTEM / (F_MAX * PRESCALER) - 1

;=====

.macro Load
ldi	 R16, @1
out	 @0, R16
.endm

.macro InitStack
Load SPL, low(RAMEND)
.endm

.macro InitTimer0
Load OCR0A,  MATCH_MAIN
Load OCR0B,  MATCH_MID
Load TCCR0A, 1<<WGM01						; CTC mode (timer clears on match OCR0A)
Load TCCR0B, 1<<CS02 | 0<<CS01 | 1<<CS00	; prescaler 1024
Load TIMSK0, 1<<OCIE0A | 1<<OCIE0B			; Enable the Compare Match interrupts
Load TIFR0,  0b11111111					; Clear the interrupt flags
.endm

.macro InitPortB
sbi DDRB, PB0
sbi PORTB, PB0
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
InitTimer0
InitPortB
DisableComparator
EnableInterrupts
.endm

.macro Run
Main:
rjmp Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0006 rjmp TimerCompareMatchA
.org 0x0007 rjmp TimerCompareMatchB

;=====

Reset:
Init
Run

;=====

TimerCompareMatchA:
sbi PORTB, PB0
reti

;=====

TimerCompareMatchB:
cbi PORTB, PB0
reti

;=====
