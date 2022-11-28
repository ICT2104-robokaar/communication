#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include <time.h>

#include <Devices/ESP8266.h>

#include <Hardware/UART_Driver.h>

#include <Devices/MSPIO.h>

#include <Hardware/CS_Driver.h>
// const char *ssid     = "Jasmin";
// const char *password = "wyj1946";

// const char *ssid     = "Marv";
// const char *password = "pky6296";

// const char *ssid     = "zhenxuan";
// const char *password = "zhenxuan";

// const char *ssid     = "Jublian";
// const char *password = "ous9306";

// const char *ssid = "CT";
// const char *password = "ajyw3712";


// DEFINE SSID and PASS
#define SSID "zhenxuan"
#define SSID_PASSWORD "zhenxuan"
#define SERVER_IP "192.168.73.54"
#define SERVER_PORT "5000"
int IRsensor1Status = 0;
int IRsensor2Status = 0;



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
// The User-defined method
int str_length(char str[]) {
    // initializing count variable (stores the length of the string)
    int count; 
    
    // incrementing the count till the end of the string
    for (count = 0; str[count] != '\0'; ++count);
    
    // returning the character count of the string
    return count; 
}

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

/*
 * Configuration for ESP8266 To Operate with:
 * Baud Rate : 115200
 * MSP432 Running at 24Mhz
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
         while (1) {
                      MSPrintf(EUSCI_A0_BASE, "Change ESP8266 Mode to Dual Mode.\r\n");
                      if (!ESP8266_ChangeMode3()) {
                        MSPrintf(EUSCI_A0_BASE, "Failed To Change ESP8266 Mode to Dual Mode\r\n");
                      } else {
                        MSPrintf(EUSCI_A0_BASE, "Successfully Changed ESP8266 Mode to Dual Mode\r\n\r\n");
                        break;
                      }
                    }
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
                     
                     char IRsensor1Value[2] = "1";
                    char IRsensor2Value[2] = "0";
                    char lWheelEncoder[2] = "0";
                    char rWheelEncoder[2] = "0";
                    // char data_send[200] = "{";
                    // strcat(data_send, "\"IRsensor1\":");
                    // strcat(data_send, IRsensor1Value);
                    // strcat(data_send, ",\"IRsensor2\":");
                    // strcat(data_send, IRsensor2Value);
                    // strcat(data_send, ",\"lWheelEncoder\":");
                    // strcat(data_send, lWheelEncoder);
                    // strcat(data_send, ",\"rWheelEncoder\":");
                    // strcat(data_send, rWheelEncoder);
                    // strcat(data_send, "}");
                    
                    
                    // time_t rawtime;
                    // struct tm * timeinfo;

                    // time ( &rawtime );
                    // timeinfo = localtime ( &rawtime );
                    // char * time = asctime (timeinfo);
                    // //Remove all new lines from time
                    // time[ str_length(time) - 1 ] = '\0';
                   


                                  while (1) {
                                                  MSPrintf(EUSCI_A0_BASE, "Start Of Program \r\n\r\n");
                                                     //NO COMMANDS
                                                      MSPrintf(EUSCI_A0_BASE, "Sending Data to Server\r\n\r\n");
                                                      while (1) {
                                                        if (!ESP8266_EstablishConnection('0', TCP, SERVER_IP, SERVER_PORT)) {
                                                          MSPrintf(EUSCI_A0_BASE, "Failed to connect to: %s:%s\r\nERROR: %s\r\n", SERVER_IP, SERVER_PORT, ESP8266_Data);
                                                        } else {
                                                          MSPrintf(EUSCI_A0_BASE, "Connected to: %s:%s\r\n\r\n", SERVER_IP, SERVER_PORT);
                                                          break;
                                                        }
                                                      }
                                                                          time_t current_time;
                    time(&current_time);
                    char * time = ctime(&current_time);
                    //Remove all new lines from time
                    time[ str_length(time) - 1 ] = '\0';

                    


                    MSPrintf(EUSCI_A0_BASE, "wheel encoder value size: %d\r\n", str_length(IRsensor1Value));
                    char data_send[300] = "{\"IRsensor1\":%s,\"IRsensor2\":%s,\"lWheelEncoder\":%s,\"rWheelEncoder\":%s,\"time\":%s}";

                    snprintf(data_send, sizeof(data_send), "{\"IRsensor1\":%s,\"IRsensor2\":%s,\"lWheelEncoder\":%s,\"rWheelEncoder\":%s,\"time\":\"%s\",\"device\":\"ESP8266\"}", IRsensor1Value, IRsensor2Value, lWheelEncoder, rWheelEncoder, time);
                    
                    //Calculating the length of the pay
                    int total_size_of_data = strlen(IRsensor1Value) + strlen(IRsensor2Value) + strlen(lWheelEncoder) + strlen(rWheelEncoder)+90+strlen(time);

                    MSPrintf(EUSCI_A0_BASE, "%s", data_send);

                                  /*Subtract one to account for the null character*/
                                  uint32_t Payload_Size = total_size_of_data;
                                  char * ipd = "+IPD";

                                  int i;
                                  int indexOfColon;
                                  int payloadLength;
                                                      if (!ESP8266_SendData('0', data_send, Payload_Size)) {
                                                        MSPrintf(EUSCI_A0_BASE, "Failed to send data \r\n");
                                                      } else {
                                                        MSPrintf(EUSCI_A0_BASE, "Data Received: %s\r\n", ESP8266_Data);
                                                        if (strstr(ESP8266_Data, ipd) == NULL) {
                                                          MSPrintf(EUSCI_A0_BASE, "No Data Received\r\n");
                                                        }
                                                      }
                                                  }
                                                }






//





