/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * Debugging log via MSP432 UART
 *
 * Description: This demo prints debugging log via a PC serial port (i.e. COM).
 * SMCLK/DCO is used as clock source and auto-clock enable feature is used by eUSCI.
 * COM used is described as "XDS110 Class Application/User UART".
 * Check your device manager to find out which COM port is used, e.g. COM3
 *
 *               MSP432P401
 *             -----------------
 *            |                 |
 *            |                 |
 *       RST -|     P1.3/UCA0TXD|----> PC
 *            |                 |
 *            |     P1.2/UCA0RXD|<---- PC
 *            |                 |
 *
 *******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <MSPIO.h>
#include <stdio.h>
#include <wchar.h>
#include <time.h>
#include <string.h>

void uPrintf(unsigned char * TxArray);

int str_length(char str[]) {
    // initializing count variable (stores the length of the string)
    int count;

    // incrementing the count till the end of the string
    for (count = 0; str[count] != '\0'; ++count);

    // returning the character count of the string
    return count;
}

//![Simple UART Config]
/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate. These
 * values were calculated using the online calculator that TI provides at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
eUSCI_UART_ConfigV1 UART0Config = {
  EUSCI_A_UART_CLOCKSOURCE_SMCLK,
  13,
  0,
  37,
  EUSCI_A_UART_NO_PARITY,
  EUSCI_A_UART_LSB_FIRST,
  EUSCI_A_UART_ONE_STOP_BIT,
  EUSCI_A_UART_MODE,
  EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};
eUSCI_UART_ConfigV1 UART2Config =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     13,
     0,
     37,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};
//![Simple UART Config]
int status = 0;

static void Delay(uint32_t loop)
    {
        volatile uint32_t i;

        for (i = 0 ; i < loop ; i++);
    }

int main(void)
{
    /* Halting WDT  */
    WDT_A_holdTimer();
    /*Initialize required hardware peripherals for the ESP8266*/
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
                GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
        MAP_UART_initModule(EUSCI_A0_BASE, &UART0Config);
        MAP_UART_enableModule(EUSCI_A0_BASE);
        MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
        MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
    /* Selecting P3.3 and P3.2 in UART mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setting DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    /* Configuring UART Module */
    UART_initModule(EUSCI_A2_BASE, &UART2Config);

    /* Enable UART module */
    UART_enableModule(EUSCI_A2_BASE);

    /* Enabling interrupts (Rx) */
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);
    Interrupt_enableMaster();
    /* Transmit data */

    while(1)
    {
            /* Transmit data */
            char IRsensor1Value[2] = "1";
            char IRsensor2Value[2] = "0";
            char lWheelEncoder[2] = "0";
            char rWheelEncoder[2] = "0";
            time_t current_time;
            time(&current_time);
            char * time = ctime(&current_time);
            //Remove all new lines from time
            time[ str_length(time) - 1 ] = '\0';
            
            MSPrintf(EUSCI_A2_BASE, "{\"IRsensor1\":%s,\"IRsensor2\":%s,\"lWheelEncoder\":%s,\"rWheelEncoder\":%s,\"time\":\"%s\",\"device\":\"m5stick\"}\n", IRsensor1Value, IRsensor2Value, lWheelEncoder, rWheelEncoder, time);
            //wait for 10 seconds
            __delay_cycles(10000000);
        }

    }



