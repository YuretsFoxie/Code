.device  ATmega16A
.include "m16Adef.inc"

.equ XTAL	      	= 16000000
.equ baudrate     	= 57600
.equ uartSetting	= XTAL / (16 * baudrate) - 1

.def flags			= R18

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

.macro InitADC
Load ADMUX, 1<<REFS1 | 1<<REFS0 | 1<<ADLAR | 1<<MUX4 | 0<<MUX3 | 0<<MUX2 | 0<<MUX1 | 0<<MUX0
Load ADCSRA, 1<<ADEN | 0<<ADSC | 0<<ADATE | 1<<ADIF | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0
.endm

.macro InitTimer
Load ASSR, 1<<AS2						; use the external clock oscillator as the clock source for the Timer/Counter2 (async mode)
Load TCCR2, 0<<CS22 | 0<<CS21 | 1<<CS20	; no prescaling (f = 32 768 Hz, 1/128 of second)
Load TIMSK, 1<<TOIE2					; Timer/Counter2 Overflow Interrupt Enable
Load TIFR, 0b11111111					; clear all interrupt flags
.endm

.macro DisableUnused
Load ACSR, 1<<ACD | 0<<ACIE
.endm

.macro SetFlags
sei
.endm

;=====

.macro Init
InitStack
InitUART
InitADC
InitTimer
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
.org 0x0016 jmp UARTReceivingIsCompleted
.org 0x001c	jmp ADCConversionIsCompleted

;=====

Reset:
Init
Run

;=====

OnTimer2Overflow:
cpi  flags, 0b00000001
breq IsFlagSet
reti

IsFlagSet:
out UDR, R19
inc R19
reti

;=====

UARTReceivingIsCompleted:
in   R16, UDR
cpi  R16, '1'
breq OnCommandStart

cpi  R16, '0'
breq OnCommandStop
reti

OnCommandStart:
ldi  flags, 0b00000001
reti

OnCommandStop:
ldi  flags, 0b00000000
reti

;=====

ADCConversionIsCompleted:
reti

;=====
