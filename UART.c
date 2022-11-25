#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Other Includes */
#include <Devices/ESP8266.h>
#include <Hardware/UART_Driver.h>
#include <string.h>
#include <PWM.h>

// IP Address of Web Server in the same network
char HTTP_WebPage[] = "172.30.141.47";

// Port 5000 will be used for Web Server
char SERVER_PORT[] = "5000";

// Data will be sent to this path of the Web Server
char HTTP_Request[] = "GET /dashboard/ \r\n\r\n";

/*Subtract one to account for the null character*/
uint32_t HTTP_Request_Size = sizeof(HTTP_Request) - 1;

// To allow for recasting of the pointer
char *received_data;

eUSCI_UART_ConfigV1 UART0Config = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK, 1, 10, 0,
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_LSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION};

eUSCI_UART_ConfigV1 UART2Config = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK, 1, 10, 0,
    EUSCI_A_UART_NO_PARITY,
    EUSCI_A_UART_LSB_FIRST,
    EUSCI_A_UART_ONE_STOP_BIT,
    EUSCI_A_UART_MODE,
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION};

static void Delay(uint32_t loop)
{
        volatile uint32_t i;

        for (i = 0; i < loop; i++)
                ;
}

/*Initialize required hardware peripherals for the ESP8266*/
MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
MAP_UART_initModule(EUSCI_A0_BASE, &UART0Config);
MAP_UART_enableModule(EUSCI_A0_BASE);
MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
MAP_Interrupt_enableInterrupt(INT_EUSCIA0);

MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
MAP_UART_initModule(EUSCI_A2_BASE, &UART2Config);
MAP_UART_enableModule(EUSCI_A2_BASE);
MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
MAP_Interrupt_enableInterrupt(INT_EUSCIA2);

/*Reset GPIO of the ESP8266*/
GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);

MAP_Interrupt_enableMaster();

/*Hard reset & flush to get rid of junk inputs*/
ESP8266_HardReset();
__delay_cycles(48000000);
UART_Flush(EUSCI_A0_BASE);
UART_Flush(EUSCI_A2_BASE);

/*Pointer to ESP8266 global buffer and assign to a variable*/
char *ESP8266_Data = ESP8266_GetBuffer();
received_data = ESP8266_Data;

/*Check UART connection to MSP432*/
if (!ESP8266_CheckConnection())
{
        MSPrintf(EUSCI_A0_BASE, "Failed MSP432 UART connection\r\n");
        /*Trap MSP432 if failed connection*/
        while (1)
                ;
}

void main()
{
        MAP_WDT_A_holdTimer();

        /*MSP432 Running at 24 MHz*/
        CS_Init();

        /*Initialize required hardware peripherals for the ESP8266*/
        /*We are using UART A0 for MSPIO*/
        UART_Init(EUSCI_A0_BASE, UART0Config);
        /*UART A2 is used for the ESP8266*/
        UART_Init(EUSCI_A2_BASE, UART2Config);
        /*Reset GPIO of the ESP8266*/
        GPIO_Init(GPIO_PORT_P6, GPIO_PIN1);

        MAP_Interrupt_enableMaster();
}
