.device ATmega8
.include "m8def.inc"

.equ mcuFrequency	= 1000000
.equ uartBaudrate	= 4800
.equ uartSetting	= mcuFrequency / (16 * uartBaudrate) - 1

.equ twiFrequency	= 50000
.equ twiPrescaler	= 1
.equ twiSetting		= (mcuFrequency / twiFrequency - 16) / (2 * twiPrescaler)

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

.macro SetBit
lds R16, @0
sbr R16, 1<<@1
sts @0, R16
.endm

.macro ClearBit
lds R16, @0
sbr R16, 0<<@1
sts @0, R16
.endm

.macro CheckTWIState
in   R16, TWSR
andi R16, 0xF8  ; Mask the prescaler bits
.endm

.macro TWIStartTransmitted
cpi  R16, 0x08
breq OnTWIStartTransmitted
.endm

.macro TWIRepeatedStartTransmitted
cpi  R16, 0x10
breq OnTWIStartTransmitted
.endm

.macro TWISLAWTransmitted
cpi  R16, 0x18
breq OnTWISLAWTransmitted
.endm

.macro TWIDataTransmitted
cpi  R16, 0x28
breq OnTWIDataTransmitted
.endm

.macro TWISLARTransmitted
cpi  R16, 0x40
breq OnTWISLARTransmitted
.endm

.macro TWIDataReceivedACK
cpi  R16, 0x50
breq OnTWIDataReceivedACK
.endm

.macro TWIError
rjmp OnTWIError
.endm

.macro InitStack
Load SPL, Low(RAMEND)
Load SPH, High(RAMEND)
.endm

.macro InitPorts
sbi DDRB, PB0
sbi DDRB, PB1
sbi DDRB, PB2
cbi PORTB, PB0
sbi PORTB, PB1
cbi PORTB, PB2
.endm

.macro InitUART
Load UBRRL, low(uartSetting)
Load UBRRH, high(uartSetting)
Load UCSRA, 0
Load UCSRB, 1<<RXEN | 1<<TXEN | 1<<RXCIE | 0<<TXCIE | 0<<UDRIE
Load UCSRC, 1<<URSEL | 1<<UCSZ0 | 1<<UCSZ1
.endm

.macro InitTWI
Load TWBR, twiSetting
Load TWSR, 0<<TWPS1 | 0<<TWPS0		; Set prescaler to 1 
;Load TWCR, 1<<TWINT | 1<<TWEA | 1<<TWEN | 1<<TWIE
Load TWCR, 1<<TWEN | 1<<TWIE
.endm

.macro InitTimer
Load TCCR0, (1<<CS02) | (1<<CS00)	; Prescaler 1024
Load TIMSK, (1<<TOIE0)				; Enable overflow interrupt
.endm

.macro DisableUnused
Load ACSR, 1<<ACD
.endm

.macro DisableInterrupts
cli
.endm

.macro EnableInterrupts
sei
.endm

.macro Init
InitStack
InitPorts
InitUART
InitTWI
InitTimer
DisableUnused
EnableInterrupts
.endm

.macro Run
Main:
DisableInterrupts
InvertBits PORTB, 0b00000001
EnableInterrupts
rjmp Main
.endm

;=====

.org 0x0000 rjmp Reset
.org 0x0009	rjmp OnTimer0Overflow
.org 0x000b rjmp OnUARTReceived
.org 0x0011 rjmp OnTWIEvent

;=====

Reset:
Init
Run

OnTimer0Overflow:
InvertBits PORTB, 0b00000110
Load TCNT0, 246					; 50 Hz for prescaler 1024
reti

OnUARTReceived:
in R16, UDR
SetBit  TWCR, TWSTA
SetBit  TWCR, TWINT
reti

OnTWIEvent:
Load UDR, 'A'

CheckTWIState
TWIStartTransmitted
TWIRepeatedStartTransmitted
TWISLAWTransmitted
TWIDataTransmitted
TWISLARTransmitted
TWIDataReceivedACK
TWIError
reti

OnTWIStartTransmitted:
Load UDR, 0b00000001
reti

OnTWIRepeatedStartTransmitted:
Load UDR, 0b00000010
reti

OnTWISLAWTransmitted:
Load UDR, 0b00000100
reti

OnTWIDataTransmitted:
Load UDR, 0b00001000
reti

OnTWISLARTransmitted:
Load UDR, 0b00010000
reti

OnTWIDataReceivedACK:
Load UDR, 0b00100000
reti

OnTWIError:
Load UDR, 0b01000000
reti

;=====
