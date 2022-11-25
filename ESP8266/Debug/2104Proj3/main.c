#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <math.h>
#include "main.h"

#include <Devices/ESP8266.h>

#include <Hardware/UART_Driver.h>

#include <Devices/MSPIO.h>

#include <Hardware/CS_Driver.h>

volatile static uint32_t time = 0;
volatile static uint32_t seconds = 0;

// DEFINE SSID and PASS
#define SSID "zhenxuan"
#define SSID_PASSWORD "zhenxuan"
#define SERVER_IP "192.168.218.54"
#define SERVER_PORT "5000"
int IRsensor1Status = 0;
int IRsensor2Status = 0;
// char HTTP_Request[] = "POST / HTTP/1.0\r\n\r\n";
// char HTTP_Request[] = "POST / HTTP/1.0\r\nContent-Type: application/json\r\nContent-Length: 20\r\n\r\n{\"name\":\"zhenxuan\"}\r\n\r\n";
// char HTTP_Request[] = "POST / HTTP1.0\r\nContent-Type: application/json\r\n\r\nContent-Length: {\"IRsensor1\":\"1\",\"IRsensor2\":\"1\"}\r\n\r\n";
char HTTP_Request[] = "POST / HTTP/1.0\r\nContent-Type: application/json\r\nContent-Length: 34\r\n\r\n{\"IRsensor1\":\"1\",\"IRsensor2\":\"1\"}\r\n\r\n";

//send ir sensor status to server with content-length
uint32_t HTTP_Request_Size = sizeof(HTTP_Request) - 1;
// command buffer
char * command=NULL;
static volatile uint8_t RXData = 0;
static uint8_t TXData = 0;
int sensorsConnected=0;
/*
 * Configuration for ESP8266 To Operate with:
 * Baud Rate : 115200
 * MSP432 Running at 24Mhz
 */


//UART Config for Serial Terminal
eUSCI_UART_ConfigV1 UART0Config =
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

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
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

// busy waiting loop
static void Delay(uint32_t loop)
    {
        volatile uint32_t i;

        for (i = 0 ; i < loop ; i++);
    }


void main(void)

    {

    MAP_WDT_A_holdTimer();      // Stopping the Watchdog Timer
    // Set msp frequency to 24
   CS_Init();
   /*Initialise Hardware Peripherals fo r ESP8266*/
       UART_Init(EUSCI_A0_BASE, UART0Config);
       UART_Init(EUSCI_A2_BASE, UART2Config);
       GPIO_Init(GPIO_PORT_P6, GPIO_PIN1);
       MAP_Interrupt_enableMaster();
       /* ESP8266  buffer*/
         char * ESP8266_Data = ESP8266_GetBuffer();
       /*ESP8266 reset*/
       ESP8266_HardReset();


         //Delay for 2 Seconds
         __delay_cycles(48000000);
         /*flush reset data*/
         UART_Flush(EUSCI_A2_BASE);
         while (1) {
              MSPrintf(EUSCI_A0_BASE, "Checking MSP432 UART Connection\r\n");
              if (!ESP8266_CheckConnection()) {
                MSPrintf(EUSCI_A0_BASE, "Failed MSP432 UART Connection\r\n");
              } else {
                MSPrintf(EUSCI_A0_BASE, "Successfully Connected to the MSP432\r\n\r\n");
                break;
              }
            }

            /* While Loop to Change ESP8266 Mode to Dual Mode
             * Break Loop if Successfully change ESP8266 Mode to Dual Mode
             */
            while (1) {
              MSPrintf(EUSCI_A0_BASE, "Change ESP8266 Mode to Dual Mode.\r\n");
              if (!ESP8266_ChangeMode3()) {
                MSPrintf(EUSCI_A0_BASE, "Failed To Change ESP8266 Mode to Dual Mode\r\n");
              } else {
                MSPrintf(EUSCI_A0_BASE, "Successfully Changed ESP8266 Mode to Dual Mode\r\n\r\n");
                break;
              }
            }

            /* While Loop to Connect ESP8266 To Access Point
             * Break Loop if Successfully connect ESP8266 to Access Point
             */
            while (1) {
              MSPrintf(EUSCI_A0_BASE, "Connecting to Access Point...\r\n");
              if (ESP8266_ConnectToAP(SSID, SSID_PASSWORD)) {
                 GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                MSPrintf(EUSCI_A0_BASE, "Successfully Connected to Access Point\r\n\r\n");
                break;
              }
            }

            /*Enable multiple connections, up to 4 according to the internet*/
            while(1){
                if (!ESP8266_EnableMultipleConnections(true)) {
                  MSPrintf(EUSCI_A0_BASE, "Failed to set multiple connections\r\n");
                }else{
                    MSPrintf(EUSCI_A0_BASE, "Multiple connections enabled\r\n\r\n");
                    break;
                }
            }
            MSPrintf(EUSCI_A0_BASE, "Multiple connections enabled\r\n\r\n");
            if (!ESP8266_EstablishConnection('0', TCP,SERVER_IP, SERVER_PORT)) {
              MSPrintf(EUSCI_A0_BASE, "Failed to establish connection\r");
              while(1);
            }
            MSPrintf(EUSCI_A0_BASE, "Connected to :%s\r\n\r\n", SERVER_IP);

            if (!ESP8266_SendData('0', HTTP_Request, HTTP_Request_Size)) {
              MSPrintf(EUSCI_A0_BASE, "Failed to send data\r");
            }
            MSPrintf(EUSCI_A0_BASE, "Data sent: %s to %s \r\n\r\n ESP8266 Data Received: %s\r\n", HTTP_Request, SERVER_IP,ESP8266_Data);
            while (1){
              // Sends periodically
              if (!ESP8266_EstablishConnection('0',TCP,SERVER_IP,SERVER_PORT))
              {
                MSPrintf(EUSCI_A0_BASE, "Failed to connect to: %s\r\nERROR: %s\r\n", SERVER_IP, ESP8266_Data);
              }
              if (!ESP8266_SendData('0', HTTP_Request, HTTP_Request_Size)) {
                MSPrintf(EUSCI_A0_BASE, "Failed to send data\r");
              }
            }
//              char data_send[] = "Hello";
//              /*Subtract one to account for the null character*/
//              uint32_t Payload_Size = sizeof(data_send) - 1;
//              char * ipd = "+IPD";
//
//              int i;
//              int indexOfColon;
//              int payloadLength;
//

//
//              while (1) {
//                MSPrintf(EUSCI_A0_BASE, "Start Of Program \r\n\r\n");
//                //There are Commands
//                if(command!=NULL){
//                    MSPrintf(EUSCI_A0_BASE, "There are Commands\r\n\r\n");
//
//                    free(command);
//                    command=NULL;
//                }else{ //NO COMMANDS
//                    MSPrintf(EUSCI_A0_BASE, "Retrieving Instructions from Server\r\n\r\n");
//                    while (1) {
//                      if (!ESP8266_EstablishConnection('0', TCP, SERVER_IP, SERVER_PORT)) {
//                        MSPrintf(EUSCI_A0_BASE, "Failed to connect to: %s:%s\r\nERROR: %s\r\n", SERVER_IP, SERVER_PORT, ESP8266_Data);
//                      } else {
//                        MSPrintf(EUSCI_A0_BASE, "Connected to: %s:%s\r\n\r\n", SERVER_IP, SERVER_PORT);
//                        break;
//                      }
//                    }
//                    if (!ESP8266_SendData('0', data_send, Payload_Size)) {
//                      MSPrintf(EUSCI_A0_BASE, "Failed to send data \r\n");
//                    } else {
//                      MSPrintf(EUSCI_A0_BASE, "Data Received: %s\r\n", ESP8266_Data);
//                      if (strstr(ESP8266_Data, ipd) == NULL) {
//                        MSPrintf(EUSCI_A0_BASE, "No Data Received\r\n");
//                      } else {
//                        //GET THE PAYLOAD
//                        for (i = 0; ESP8266_Data[i] != '\0'; i++) {
//                          if (ESP8266_Data[i] == ':') {
//                            indexOfColon = i;
//                            break;
//                          }
//                        }
//                        payloadLength = atoi( & ESP8266_Data[indexOfColon - 1]);
//                        MSPrintf(EUSCI_A0_BASE, "%i\r\n", payloadLength);
//                        //Dynamic Allocate Memory for char Array: command
//                        command = malloc(sizeof(char) * (payloadLength + 1));
//                        for (i = 0; i < payloadLength; i++) {
//                          command[i] = ESP8266_Data[indexOfColon + 1 + i];
//                        }
//                        command[payloadLength] = '\0';
//                        MSPrintf(EUSCI_A0_BASE, "The command is : %s\r\n", command);
//                        break;
//                      }
//                    }
//                }
//              }
    /* timer to get number of thing per second */
    /* Configuring Timer_A1 for Up Mode used to sample*/




    }
//





