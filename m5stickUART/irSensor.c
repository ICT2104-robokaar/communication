/*
 * irSensor.c
 *
 *  Created on: 2 Dec 2021
 *      Author: zhenx
 */
#include "irSensor.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void init_left_irSensor(){
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, GPIO_PIN7);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN7, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN7);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN7);

}

void init_right_irSensor(){
    /* Set port pin for IR Line Detector*/
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, GPIO_PIN5);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN5);
}
