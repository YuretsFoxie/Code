.device  ATmega16A
.include "m16Adef.inc"

.equ XTAL	      	= 16000000
.equ baudrate     	= 57600
.equ uartSetting	= XTAL / (16 * baudrate) - 1

;=====

.macro Load
ldi	 R16, @1
out	 @0, R16
.endm

;=====

.macro InitStack
Load SPL, low(RAMEND)
Load SPH, high(RAMEND)
.endm

.macro InitUART
Load UBRRL, low(uartSetting)
Load UBRRH, high(uartSetting)
Load UCSRA, 0
Load UCSRB, 1<<RXEN | 1<<TXEN | 1<<RXCIE | 0<<TXCIE | 0<<UDRIE
Load UCSRC, 1<<URSEL | 1<<UCSZ0 | 1<<UCSZ1
.endm

.macro InitTimer
Load ASSR, 1<<AS2						; use the external clock oscillator as the clock source for the Timer/Counter2 (async mode)
Load TCCR2, 1<<CS22 | 0<<CS21 | 1<<CS20	; 1s, prescaler is 128 (32768 Hz / 256 = 128 Hz)
Load TIMSK, 1<<TOIE2					; Timer/Counter2 Overflow Interrupt Enable
Load TIFR, 0b11111111					; clear all interrupt flags
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
InitUART
InitTimer
DisableUnused
SetFlags
ldi R17, 0
.endm

.macro Run
Main:
rjmp Main
.endm

;=====

.org 0x0000 jmp Reset
.org 0x0008 jmp OnTimer2Overflow
.org 0x0016 jmp UARTReceivingIsCompleted

;=====

Reset:
Init
Run

;=====

OnTimer2Overflow:
inc R17
cpi R17, 60
breq OnEqual
reti

	OnEqual:
	ldi R17, 0
	ldi R16, 1
	out UDR, R16
	reti

;=====

UARTReceivingIsCompleted:
reti

;=====
