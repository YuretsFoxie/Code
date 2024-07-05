.device  ATmega16A
.include "m16Adef.inc"

.equ XTAL	      	= 16000000
.equ baudrate     	= 57600
.equ uartSetting	= XTAL / (16 * baudrate) - 1

.equ calibration	= 0b11110001	; ADC1 ADC1 x1
.equ alpha			= 0b11110000	; ADC0 ADC1 x1
.equ ecg1			= 0b11110010	; ADC2 ADC1 x1
.equ ecg2			= 0b11110011	; ADC3 ADC1 x1

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
Load ADMUX, 1<<REFS1 | 1<<REFS0 | 1<<ADLAR | 1<<MUX4 | 0<<MUX3 | 0<<MUX2 | 1<<MUX1 | 1<<MUX0	; internal 2.56V Voltage Reference, left adjusted result, differential mode (ecg2 selected, to be skiped)
Load ADCSRA, 1<<ADEN | 1<<ADSC | 0<<ADATE | 1<<ADIF | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0	; ADC conversion is started to initialize ADC, it should end before starting the main measurements
.endm

.macro InitTimer
Load ASSR, 1<<AS2						; use the external clock oscillator as the clock source for the Timer/Counter2 (async mode)
Load TCCR2, 0<<CS22 | 0<<CS21 | 1<<CS20	; no prescaling (f = 32 768 Hz, 1/128 of second)
Load TIMSK, 1<<OCIE2					; Timer/Counter2 Compare Match Interrupt Enable
Load TIFR, 0b11111111					; clear all interrupt flags
Load OCR2, 127							; 255 is max value
.endm

.macro InitSleep
Load MCUCR, 0<<SM2 | 0<<SM1 | 1<<SM0	; ADC Noise Reduction mode
.endm

.macro DisableUnused
Load ACSR, 1<<ACD | 0<<ACIE				; Analogous Comparator is off
.endm

.macro EnableInterrupts
sei
.endm

.macro ClearFlag
clt
.endm

.macro Switch
in R16, @0
.endm

.macro Case
cpi R16, @0
breq @1
.endm

.macro ResetTimer
SetBits SFIOR, 1<<PSR2
.endm

.macro EnableSleepMode
SetBits MCUCR, 1<<SE
.endm

.macro DisableSleepMode
ClearBits MCUCR, 1<<SE
.endm

.macro Transmit
in R16, @0
out UDR, R16
.endm

.macro SetFlag
set
.endm

;=====

.macro Init
InitStack
InitUART
InitADC
InitTimer
InitSleep
DisableUnused
EnableInterrupts
.endm

.macro Run
Main: sleep
rjmp Main
.endm

;=====

.org 0x0000 jmp Reset
.org 0x0006 jmp Timer2CompareMatch
.org 0x0016 jmp UARTReceivingIsCompleted
.org 0x001c	jmp ADCConversionIsCompleted

;=====

Reset:
Init
Run

;=====

Timer2CompareMatch:
brts IsFlag
reti

IsFlag:
EnableSleepMode
reti

;=====

UARTReceivingIsCompleted:
Switch UDR
Case '1', OnCommandStart
Case '0', OnCommandStop
reti

OnCommandStart:
ResetTimer
EnableSleepMode
SetFlag
reti
	
OnCommandStop:
ClearFlag
reti

;=====

ADCConversionIsCompleted:
DisableSleepMode
Transmit ADCH

Switch MCUCR
Case calibration,	SetAlpha
Case alpha,			SetECG1
Case ecg1, 			SetECG2
Case ecg2, 			SetCalibration
reti

SetCalibration:
Load MCUCR, calibration
reti

SetAlpha:
Load MCUCR, alpha
reti

SetECG1:
Load MCUCR, ecg1
reti

SetECG2:
Load MCUCR, ecg2
reti

;=====
