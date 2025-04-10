Please, provide the complete description of the algorithm instead of the code. 

Certainly! Here's a complete description of the algorithm for interfacing an ATmega8 AVR MCU with a DS1307 RTC chip using TWI (I2C) and handling time setting and reading via interrupts.
Algorithm Description
Initialization

    Setup Stack Pointer:
        Initialize the stack pointer to the end of SRAM.

    TWI (I2C) Initialization:
        Set the TWI bit rate register (TWBR) to configure the SCL clock frequency.
        Enable the TWI module and its interrupt by setting TWEN and TWIE bits in the TWCR register.

    External Interrupt Initialization:
        Configure the external interrupt (INT0) to trigger on a falling edge by setting the ISC01 bit in the MCUCR register.
        Enable the external interrupt by setting the INT0 bit in the GICR register.

    Global Interrupt Enable:
        Enable global interrupts by setting the I-bit in the SREG register.

Setting Initial Time on DS1307

    Send Start Condition:
        Set the TWSTA bit in the TWCR register to send a start condition.
        Wait for the TWINT flag to be set, indicating that the start condition has been transmitted.

    Send DS1307 Address + Write Bit:
        Load the DS1307 address (0xD0) into the TWDR register.
        Clear the TWSTA bit and set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the address has been transmitted and acknowledged.

    Send Register Address (Seconds Register):
        Load the seconds register address (0x00) into the TWDR register.
        Set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the register address has been transmitted and acknowledged.

    Send Initial Time Data:
        Load the initial seconds value (e.g., 0x00) into the TWDR register and transmit.
        Load the initial minutes value (e.g., 0x59) into the TWDR register and transmit.
        Load the initial hours value (e.g., 0x12) into the TWDR register and transmit.

    Send Stop Condition:
        Set the TWSTO bit in the TWCR register to send a stop condition.
        Wait for the TWSTO bit to be cleared by hardware, indicating that the stop condition has been executed.

Handling External Interrupt (Reading Time)

    External Interrupt Service Routine (ISR) for INT0:
        Save the current register values to the stack.
        Call the function to read the time from the DS1307.
        Restore the register values from the stack.
        Return from the interrupt.

Reading Time from DS1307

    Send Start Condition:
        Set the TWSTA bit in the TWCR register to send a start condition.
        Wait for the TWINT flag to be set, indicating that the start condition has been transmitted.

    Send DS1307 Address + Write Bit:
        Load the DS1307 address (0xD0) into the TWDR register.
        Clear the TWSTA bit and set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the address has been transmitted and acknowledged.

    Send Register Address (Seconds Register):
        Load the seconds register address (0x00) into the TWDR register.
        Set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the register address has been transmitted and acknowledged.

    Send Repeated Start Condition:
        Set the TWSTA bit in the TWCR register to send a repeated start condition.
        Wait for the TWINT flag to be set, indicating that the repeated start condition has been transmitted.

    Send DS1307 Address + Read Bit:
        Load the DS1307 address with the read bit (0xD1) into the TWDR register.
        Clear the TWSTA bit and set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the address has been transmitted and acknowledged.

    Read Time Data:
        Read the seconds value from the TWDR register.
        Read the minutes value from the TWDR register.
        Read the hours value from the TWDR register.

    Send Stop Condition:
        Set the TWSTO bit in the TWCR register to send a stop condition.
        Wait for the TWSTO bit to be cleared by hardware, indicating that the stop condition has been executed.

Main Loop

    Keep the main loop empty as all the work is handled in the interrupt service routines.

Summary

    Initialize TWI (I2C) and external interrupts.
    Set the initial time on the DS1307 RTC.
    Use external interrupt to trigger reading the current time from the DS1307.
    Handle TWI communication using interrupts to read and write data to/from the DS1307.

This algorithm outlines the steps needed to set the time on the DS1307 RTC and read the time whenever an external interrupt occurs, using TWI (I2C) communication and interrupt service routines.

==========

"Wait for the TWINT flag to be set, indicating that the start condition has been transmitted." Does it mean to wait for the interrupt?

Yes, waiting for the TWINT flag to be set indeed means waiting for the interrupt. The TWINT (TWI Interrupt Flag) is set when the TWI hardware completes a byte transfer, signaling that the next action can be taken. Here's how it works in the context of TWI communication:
TWI (I2C) Communication Flow

    Start Condition:
        Initiate a start condition by setting the TWSTA bit and clearing the TWINT bit in the TWCR (TWI Control Register).
        The TWI hardware will set the TWINT flag once the start condition has been transmitted, and an interrupt will be triggered if the TWIE (TWI Interrupt Enable) bit is set.

    Sending Address and Data:
        Load the address or data byte into the TWDR (TWI Data Register).
        Clear the TWINT bit to start the transmission.
        The TWI hardware will set the TWINT flag once the byte has been transmitted and acknowledged, triggering an interrupt if the TWIE bit is set.

    Reading Data:
        Clear the TWINT bit to start reading.
        The TWI hardware will set the TWINT flag once the byte has been received, triggering an interrupt if the TWIE bit is set.

    Stop Condition:
        Initiate a stop condition by setting the TWSTO bit and clearing the TWINT bit.
        The TWINT flag is not set after a stop condition, so no interrupt is generated.

Interrupt Handling

In the interrupt service routine (ISR), you check the TWI status register (TWSR) to determine what action to take next based on the current state of the TWI communication.
Example of Handling TWINT in an ISR

Here's how you might handle the TWINT flag in an ISR:
Assembly

; TWI Interrupt Service Routine
TWI_ISR:
    in temp, TWSR
    andi temp, 0xF8  ; Mask the prescaler bits

    cpi temp, 0x08   ; Check for START condition transmitted
    breq TWI_Start_Transmitted
    cpi temp, 0x10   ; Check for repeated START condition transmitted
    breq TWI_Start_Transmitted
    cpi temp, 0x18   ; Check for SLA+W transmitted, ACK received
    breq TWI_SLA_W_Transmitted
    cpi temp, 0x28   ; Check for data byte transmitted, ACK received
    breq TWI_Data_Transmitted
    cpi temp, 0x40   ; Check for SLA+R transmitted, ACK received
    breq TWI_SLA_R_Transmitted
    cpi temp, 0x50   ; Check for data byte received, ACK returned
    breq TWI_Data_Received_ACK
    cpi temp, 0x58   ; Check for data byte received, NACK returned
    breq TWI_Data_Received_NACK

    reti

TWI_Start_Transmitted:
    ; Load SLA+W or SLA+R into TWDR and transmit
    cpi twi_state, TWI_READ_SEC_STATE
    breq TWI_Start_Read

    ldi temp, DS1307_ADDR
    rcall TWI_Write
    reti

TWI_Start_Read:
    ldi temp, DS1307_ADDR + 1
    rcall TWI_Write
    reti

TWI_SLA_W_Transmitted:
    ; Load data byte into TWDR and transmit
    cpi twi_state, TWI_READ_SEC_STATE
    breq TWI_Write_RegAddr
    cpi twi_state, TWI_READ_MIN_STATE
    breq TWI_Write_RegAddr
    cpi twi_state, TWI_READ_HR_STATE
    breq TWI_Write_RegAddr

    ; Default case for setting time
    cpi twi_state, TWI_IDLE_STATE
    breq TWI_Write_Sec

    reti

TWI_Write_RegAddr:
    ldi temp, DS1307_SEC_REG
    rcall TWI_Write
    reti

TWI_Write_Sec:
    ldi temp, 0x00  ; Seconds
    rcall TWI_Write
    ldi twi_state, TWI_WRITE_MIN_STATE
    reti

TWI_Write_Min:
    ldi temp, 0x59  ; Minutes
    rcall TWI_Write
    ldi twi_state, TWI_WRITE_HR_STATE
    reti

TWI_Write_Hr:
    ldi temp, 0x12  ; Hours (24-hour mode)
    rcall TWI_Write
    ldi twi_state, TWI_IDLE_STATE
    rcall TWI_Stop
    reti

TWI_Data_Transmitted:
    ; Load next data byte or issue STOP condition
    cpi twi_state, TWI_WRITE_MIN_STATE
    breq TWI_Write_Min
    cpi twi_state, TWI_WRITE_HR_STATE
    breq TWI_Write_Hr

    rcall TWI_Stop
    reti

TWI_SLA_R_Transmitted:
    ; Prepare to receive data byte
    rcall TWI_Read_ACK
    reti

TWI_Data_Received_ACK:
    ; Store received data and prepare for next byte
    cpi twi_state, TWI_READ_SEC_STATE
    breq TWI_Store_Sec
    cpi twi_state, TWI_READ_MIN_STATE
    breq TWI_Store_Min

    ; Default case to read hours
    mov hours, temp
    ldi twi_state, TWI_IDLE_STATE
    rcall TWI_Read_NACK
    reti

TWI_Store_Sec:
    mov seconds, temp
    ldi twi_state, TWI_READ_MIN_STATE
    rcall TWI_Read_ACK
    reti

TWI_Store_Min:
    mov minutes, temp
    ldi twi_state, TWI_READ_HR_STATE
    rcall TWI_Read_NACK
    reti

TWI_Data_Received_NACK:
    ; Store received data and issue STOP condition
    mov hours, temp
    ldi twi_state, TWI_IDLE_STATE
    rcall TWI_Stop
    reti

Summary

    Waiting for the TWINT flag: This means waiting for the corresponding interrupt to be triggered, indicating that the TWI operation has been completed.
    Handling in ISR: The ISR checks the TWI status and takes appropriate actions based on the current state of the TWI communication.

This approach ensures that TWI operations are handled efficiently using interrupts, allowing the main program to remain responsive and perform other tasks.

==========

Please, don't provide the code just yet, just answer. "Clear the TWSTA bit and set the TWINT bit in the TWCR register to start transmission." Do I understand correctly, that it is required to set the TWINT bit (interrupt flag) manually?

Yes, you understand correctly. In TWI (I2C) communication on the ATmega8, after setting the start condition, you need to manually set the TWINT bit in the TWCR (TWI Control Register) to clear the interrupt flag and start the transmission. This action indicates to the TWI hardware that it should begin the next operation.

Specifically, after initiating a start condition by setting the TWSTA bit and clearing the TWINT bit, the TWI hardware will complete the start condition and set the TWINT bit. You then need to clear the TWINT bit manually to signal that you are ready for the next step in the communication process (such as sending the address or data).

Here’s a step-by-step summary:

    Set the TWSTA bit to initiate a start condition.
    Clear the TWINT bit to begin the start condition transmission.
    Wait for the TWINT bit to be set by the hardware, indicating the start condition has been transmitted.
    Load the address or data into the TWDR register.
    Clear the TWINT bit to start the transmission of the address or data.

By manually clearing the TWINT bit, you are effectively acknowledging the completion of the previous action and instructing the TWI hardware to proceed with the next step.
"Clear the TWSTA bit and set the TWINT bit in the TWCR register to start transmission." or "Clear the TWINT bit to start the transmission of the address or data."? So, should I write 0 or 1 to TWINT?

To clear the TWINT (TWI Interrupt Flag) bit, you actually need to write a 1 to it. This might seem counterintuitive, but that's how the TWI hardware is designed to acknowledge the completion of the current operation and to start the next operation.

So, the correct action is to write a 1 to TWINT to clear it and start the transmission of the address or data.
Summary

    Set the TWINT bit to 1 to clear the interrupt flag and start the transmission of the address or data.
    Set the TWSTA bit to 0 to clear the start condition flag (after the start condition has been transmitted).

Here’s how you do it in the context of the TWI control register (TWCR):
Assembly

; Clear TWINT bit to start the transmission of address or data
ldi temp, (1<<TWINT) | (1<<TWEN) | (1<<TWIE) ; TWINT = 1, TWEN = 1, TWIE = 1
out TWCR, temp

This ensures that the TWI operation proceeds to the next step.
"Wait for the TWINT flag to be set, indicating that the address has been transmitted and acknowledged." If DS1307 is not acknowledged, will the inrerrupt be called?

If the DS1307 is not acknowledged, the TWI interrupt will still be called, but the status code in the TWSR (TWI Status Register) will indicate that the SLA+W (Slave Address + Write) or SLA+R (Slave Address + Read) was not acknowledged. This status code helps determine the next steps in the communication process.
TWI Status Codes

    SLA+W transmitted, ACK received: Status code 0x18
    SLA+W transmitted, NACK received: Status code 0x20
    SLA+R transmitted, ACK received: Status code 0x40
    SLA+R transmitted, NACK received: Status code 0x48

Handling NACK in the ISR

When the interrupt is called, you should check the TWSR to determine the status and handle NACK appropriately. If the DS1307 does not acknowledge, you can decide to retry, send a stop condition, or handle the error as needed.

Here is a brief description of handling NACK in the ISR:

    Check Status Code:
        Read the TWSR register and mask the prescaler bits (ANDI TWSR, 0xF8).

    Handle Specific Status Codes:
        If the status code indicates a NACK (e.g., 0x20 or 0x48), handle the error by retrying or sending a stop condition.

Example Workflow for Handling NACK

    Send Start Condition:
        Set TWSTA and clear TWINT.
        Wait for the TWINT flag to be set (interrupt will be called).

    Check Status in ISR:
        If the status code indicates NACK (0x20 or 0x48), handle the error.
        If the status code indicates ACK (0x18 or 0x40), proceed with sending the data or reading the data.

    Retry or Stop:
        If NACK, decide whether to retry the operation or send a stop condition to terminate the communication.

This approach ensures that your program can handle cases where the DS1307 does not acknowledge the address, allowing for robust error handling in your TWI communication.

==========

"Send Repeated Start Condition" Is it nesessary to send the Repeated Start Condition, if DS1307 is acknowledged?

No, it is not strictly necessary to send a repeated start condition if the DS1307 is acknowledged and you are continuing communication without losing control of the bus. However, a repeated start condition is typically used in the following scenarios:

    Switching from Write to Read:
        When you need to switch from writing to the DS1307 (e.g., setting the register pointer) to reading from it (e.g., reading the time data), a repeated start condition is used to avoid releasing the bus and potentially losing control to another master device.

    Ensuring Bus Control:
        If there is a possibility of losing control of the bus (e.g., due to arbitration with another master device), a repeated start condition ensures continuous control of the bus without having to send a stop condition and then re-acquiring the bus with a new start condition.

Typical Use Case in DS1307 Communication

    Setting the Register Pointer:
        Send a start condition.
        Send the DS1307 address with the write bit.
        Send the register address (e.g., seconds register).

    Reading Data: 4. Send a repeated start condition. 5. Send the DS1307 address with the read bit. 6. Read the data bytes (seconds, minutes, hours).

Example Workflow

    Send Start Condition:
        Initiate communication by sending a start condition.

    Send DS1307 Address + Write Bit:
        Send the DS1307 address with the write bit to set the register pointer.

    Send Register Address:
        Send the address of the register you want to read (e.g., seconds register).

    Send Repeated Start Condition:
        Send a repeated start condition to switch to read mode without releasing the bus.

    Send DS1307 Address + Read Bit:
        Send the DS1307 address with the read bit to read the data.

    Read Data:
        Read the data bytes from the DS1307.

Summary

    A repeated start condition is not strictly necessary if the DS1307 is acknowledged and you are continuing communication without losing control of the bus.
    However, it is commonly used to switch from write to read mode without releasing the bus, ensuring continuous control of the bus during the entire communication process.

==========

Please, add more details to this part:
   " Send DS1307 Address + Read Bit:
        Load the DS1307 address with the read bit (0xD1) into the TWDR register.
        Clear the TWSTA bit and set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the address has been transmitted and acknowledged."

Certainly! Here is an expanded and detailed description of the step where the DS1307 address with the read bit is sent:
Send DS1307 Address + Read Bit

    Purpose:
        This step is to address the DS1307 RTC chip and indicate that the following operation will be a read operation. The DS1307 will respond by sending the data from the specified register.

    Steps:

        Load the DS1307 Address with the Read Bit into the TWDR Register:

            The DS1307 has a 7-bit address, which is 0x68. When combined with the read/write bit, the 8-bit address for reading is 0xD1 (since 0x68 << 1 | 1 = 0xD1).

            The TWDR (TWI Data Register) is used to hold the address and data to be transmitted. Load the value 0xD1 into the TWDR register to indicate a read operation.
            Assembly

    ldi temp, 0xD1
    out TWDR, temp

Clear the TWSTA Bit and Set the TWINT Bit in the TWCR Register:

    The TWSTA (TWI Start Condition) bit is set to initiate a start condition. After the start condition has been transmitted, the TWSTA bit should be cleared.

    The TWINT (TWI Interrupt Flag) bit must be set to 1 to clear the interrupt flag and start the transmission of the address. This signals the TWI hardware to proceed with the next operation.

    The TWCR (TWI Control Register) is configured as follows:
        Clear the TWSTA bit to indicate that the start condition has been transmitted.
        Set the TWINT bit to clear the interrupt flag and start the transmission.
        Ensure the TWEN (TWI Enable) bit is set to enable the TWI module.
        Optionally, ensure the TWIE (TWI Interrupt Enable) bit is set if using interrupts.
    Assembly

    ldi temp, (1<<TWINT) | (1<<TWEN) | (1<<TWIE) ; TWINT = 1, TWEN = 1, TWIE = 1
    out TWCR, temp

Wait for the TWINT Flag to be Set:

    After setting the TWINT bit, the TWI hardware will begin transmitting the address.

    The TWINT flag will be set by the hardware once the address has been transmitted and acknowledged (ACK received) or not acknowledged (NACK received).

    Poll the TWINT flag to wait for it to be set, indicating that the transmission is complete. This can be done in a loop or can be handled in an interrupt service routine (ISR).
    Assembly

    wait_twint:
        in temp, TWCR
        sbrc temp, TWINT
        rjmp wait_twint

Check the TWSR (TWI Status Register) for Acknowledgment:

    Once the TWINT flag is set, check the TWSR to determine if the address was acknowledged.

    The TWSR contains a status code that indicates the result of the operation. For a successful SLA+R (Slave Address + Read) transmission, the status code should be 0x40 (SLA+R transmitted, ACK received).
    Assembly

            in temp, TWSR
            andi temp, 0xF8  ; Mask the prescaler bits
            cpi temp, 0x40   ; Check for SLA+R transmitted, ACK received

Summary of the Detailed Steps

    Load the DS1307 address with the read bit (0xD1) into the TWDR register.
    Clear the TWSTA bit and set the TWINT bit in the TWCR register to start the transmission.
    Wait for the TWINT flag to be set, indicating that the address has been transmitted.
    Check the TWSR for acknowledgment to ensure the address was successfully transmitted and acknowledged.

This detailed description ensures that each step of sending the DS1307 address with the read bit is clearly understood, and the necessary actions are taken to handle the TWI communication correctly.

==========

How do I read all the data from DS1307, not just the time?

To read all the data from the DS1307 RTC, including the time and other registers, you need to follow these steps:
DS1307 Registers Overview

The DS1307 has the following registers:

    00h: Seconds
    01h: Minutes
    02h: Hours
    03h: Day
    04h: Date
    05h: Month
    06h: Year
    07h: Control
    08h - 3Fh: RAM (56 bytes of user RAM)

Algorithm to Read All Data from DS1307
Initialization

    Setup Stack Pointer:
        Initialize the stack pointer to the end of SRAM.

    TWI (I2C) Initialization:
        Set the TWI bit rate register (TWBR) to configure the SCL clock frequency.
        Enable the TWI module and its interrupt by setting TWEN and TWIE bits in the TWCR register.

    External Interrupt Initialization:
        Configure the external interrupt (INT0) to trigger on a falling edge by setting the ISC01 bit in the MCUCR register.
        Enable the external interrupt by setting the INT0 bit in the GICR register.

    Global Interrupt Enable:
        Enable global interrupts by setting the I-bit in the SREG register.

Reading All Data from DS1307

    Send Start Condition:
        Set the TWSTA bit in the TWCR register to send a start condition.
        Clear the TWINT bit in the TWCR register to start the transmission.
        Wait for the TWINT flag to be set, indicating that the start condition has been transmitted.

    Send DS1307 Address + Write Bit:
        Load the DS1307 address (0xD0) into the TWDR register.
        Clear the TWSTA bit and set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the address has been transmitted and acknowledged.

    Send Register Address (Starting from Seconds Register - 0x00):
        Load the starting register address (0x00) into the TWDR register.
        Set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the register address has been transmitted and acknowledged.

    Send Repeated Start Condition:
        Set the TWSTA bit in the TWCR register to send a repeated start condition.
        Clear the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the repeated start condition has been transmitted.

    Send DS1307 Address + Read Bit:
        Load the DS1307 address with the read bit (0xD1) into the TWDR register.
        Clear the TWSTA bit and set the TWINT bit in the TWCR register to start transmission.
        Wait for the TWINT flag to be set, indicating that the address has been transmitted and acknowledged.

    Read Data from DS1307 Registers:

        Read the Seconds Register (0x00):
            Set the TWINT bit to start the read operation.
            Wait for the TWINT flag to be set, indicating that the data byte has been received.
            Read the data from the TWDR register.

        Repeat the Read Operation for Remaining Registers (0x01 to 0x3F):
            For each subsequent register, set the TWINT bit to start the read operation.
            Wait for the TWINT flag to be set, indicating that the data byte has been received.
            Read the data from the TWDR register.
            Continue this process for all the registers from 0x00 to 0x3F.

    Send Stop Condition:
        Set the TWSTO bit and clear the TWINT bit in the TWCR register to send a stop condition.
        Wait for the TWSTO bit to be cleared by hardware, indicating that the stop condition has been executed.

Summary

    Initialization: Set up the stack pointer, TWI module, external interrupts, and enable global interrupts.
    Send Start Condition: Initiate communication with the DS1307.
    Send DS1307 Address + Write Bit: Address the DS1307 to indicate a write operation.
    Send Register Address: Specify the starting register address (0x00) to read from.
    Send Repeated Start Condition: Switch to read mode without releasing the bus.
    Send DS1307 Address + Read Bit: Address the DS1307 to indicate a read operation.
    Read Data from DS1307: Sequentially read data from all registers (0x00 to 0x3F).
    Send Stop Condition: Terminate communication with the DS1307.

This process ensures that all data, including time and RAM, is read from the DS1307 RTC chip.

==========

This sets up the SCL frequency to approximately 50 kHz for a 1 MHz CPU clock.

TWBR = (1000000 / 50000 - 16) / (2 * 1)
TWBR = (20 - 16) / 2
TWBR = 4 / 2
TWBR = 2

Set the prescaler to 1 (TWPS1=0, TWPS0=0)
TWSR = 0x00;

Set TWBR (TWI Bit Rate Register) for 50 kHz SCL frequency
TWBR = 2;

Enable TWI
TWCR = (1 << TWEN);



==========

DS1307 RTC must have an external 3 V micro battery in addition to the main rechargable battery or engine.

Table in SRAM contains all the RTC registers data: time, date and control.
If the table values are changed, they are written to the RTC and displayed at the watch.
The values on the watch, SRAM table and RTC must be syncronized.
At least control register needs an initial value to generate the pulse each second. The other values may be set manually.
In time setting mode the table values may be changed.

The rotation knob may be used to scroll (in basic mode), switch modes (with the fast reverse gesture), change values (in setting mode) and charge the watch.
The knob must rotate a micro generator.









