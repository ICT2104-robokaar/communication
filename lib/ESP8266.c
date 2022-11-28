#include <Hardware/CS_Driver.h>
#include <Devices/MSPIO.h>
#include <Devices/ESP8266.h>

// IP Address of Web Server in the same network
char HTTP_WebPage[] = "172.20.10.3";

// Port 8000 will be used
char Port[] = "8000";

// Data will be sent to this path of the Web Server
char HTTP_Request[] = "GET /src/mvc/view/receiver.php?speed=123 HTTP/1.0\r\n\r\n";

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