.device  ATmega16A
.include "m16Adef.inc"

.equ XTAL	      	= 16000000
.equ baudrate     	= 250000
.equ uartSetting	= XTAL / (16 * baudrate) - 1

.def flags			= R18

;=====

.macro Load
ldi	 R16, @1
out	 @0, R16
.endm

.macro SetBits
in   R16, @0
ori  R16, @1
out  @0, R16
.endm

.macro ClearBits
in   R16, @0
ldi  R17, @1
com  R17
and  R16, R17
out  @0, R16
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

.macro InitSleep
Load MCUCR, 1<<SM0
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
InitSleep
DisableUnused
SetFlags
.endm

.macro Run
Main: 
sleep
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
	SetBits MCUCR, 1<<SE
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
	SetBits SFIOR, 1<<PSR2
	SetBits MCUCR, 1<<SE
	ldi  flags, 0b00000001
	reti
	
	OnCommandStop:
	ldi  flags, 0b00000000
	reti
	
;=====

ADCConversionIsCompleted:
ClearBits MCUCR, 1<<SE
in   R16, ADCH
out  UDR, R16
reti

;=====

; 2.56V:
; step: 0.02

; s110 0110 -> 102
; 102 x 2.56 = 261.12
; 261.12 / 128 = 2.04

; s110 0111 -> 103
; 103 x 2.56 = 263.68
; 263.68 / 128 = 2.06

;=====

; ADC

; ADMUX: REFS1, REFS0, ADLAR, MUX4, MUX3, MUX2, MUX1, MUX0

; 0<<REFS1 | 0<<REFS0	: AREF, Internal Vref turned off
; 0<<REFS1 | 1<<REFS0  	: AVCC with external capacitor at AREF pin
; 1<<REFS1 | 1<<REFS0 	: Internal 2.56V Voltage Reference with external capacitor at AREF pin

; 1<<ADLAR										  	: left aligned result (ADCH only)
; 1<<MUX4 | 0<<MUX3 | 0<<MUX2 | 0<<MUX1 | 0<<MUX0	: differential mode, positive input: ADC0, negative input: ADC1, gain: x1



; ADCSRA: ADEN, ADSC, ADATE, ADIF, ADIE, ADPS2, ADPS1, ADPS0

; 1<<ADEN							: ADC is enabled
; 0<<ADSC							: conversion is not started
; 0<<ADATE							: auto triggering is disabled
; 1<<ADIF							: inerrupt flag is cleared
; 1<<ADIE							: interrupt is enabled
; 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0	: prescaler is 128: 125 kHz



; SFIOR: ADTS2, ADTS1, ADTS0, – , ACME, PUD, PSR2, PSR10

; 0<<ADTS2 | 0<<ADTS1 | 0<<ADTS0	: Free Running mode
; 0<<ADTS2 | 0<<ADTS1 | 1<<ADTS0 	: Analog Comparator
; 0<<ADTS2 | 1<<ADTS1 | 0<<ADTS0 	: External Interrupt Request 0
; 0<<ADTS2 | 1<<ADTS1 | 1<<ADTS0 	: Timer/Counter0 Compare Match
; 1<<ADTS2 | 0<<ADTS1 | 0<<ADTS0 	: Timer/Counter0 Overflow
; 1<<ADTS2 | 0<<ADTS1 | 1<<ADTS0 	: Timer/Counter1 Compare Match B
; 1<<ADTS2 | 1<<ADTS1 | 0<<ADTS0 	: Timer/Counter1 Overflow
; 1<<ADTS2 | 1<<ADTS1 | 1<<ADTS0 	: Timer/Counter1 Capture Event

; 1<<ACME							: Analog Comparator Multiplexed Input (ADC must be switched off)
; 1<<PUD							: disable all pull-ups in all ports
; 1<<PSR2							: Prescaler Reset Timer/Counter2
; 1<<PSR10							: Prescaler Reset Timer/Counter1 and Timer/Counter0

;=====

; Timer

; TCCR2: CS22 CS21 CS20

; CS22 CS21 CS20 Description
; 0<<CS22 | 0<<CS21 | 0<<CS20	: No clock source (Timer/Counter2 stopped)
; 0<<CS22 | 0<<CS21 | 1<<CS20 	: No prescaling
; 0<<CS22 | 1<<CS21 | 0<<CS20	: prescaler is 8
; 0<<CS22 | 1<<CS21 | 1<<CS20	: prescaler is 32
; 1<<CS22 | 0<<CS21 | 0<<CS20	: prescaler is 64
; 1<<CS22 | 0<<CS21 | 1<<CS20	: prescaler is 128
; 1<<CS22 | 1<<CS21 | 0<<CS20	: prescaler is 256
; 1<<CS22 | 1<<CS21 | 1<<CS20	: prescaler is 1024
