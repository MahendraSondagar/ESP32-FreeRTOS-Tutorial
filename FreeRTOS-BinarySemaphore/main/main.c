/* FreeRTOS Simple binary sempahore
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "driver/gpio.h"
#include "sdkconfig.h"


TaskHandle_t BossTh=NULL;
TaskHandle_t EmployeeTh=NULL;
SemaphoreHandle_t EmployeeSignal=NULL;
/*
 * Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/


/***********************************
 *FreeRTOS Simple binarySemaphore  
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
/**********************************/

#define BLINK_GPIO CONFIG_BLINK_GPIO

static void Toggle_Led(void)
{
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
}

void BossThread(void *p)
{
	for(;;)
	{
		xSemaphoreGive(EmployeeSignal);
		printf("Boss Sent the signal\r\n");
		vTaskDelay(500/portTICK_PERIOD_MS);
	}

}

void EmployeeThread(void *p)
{
	for(;;)
	{
		if(xSemaphoreTake(EmployeeSignal, portMAX_DELAY))
		{
			printf("Employee Received The signal\r\n");
			Toggle_Led();
		}

	}
}



void app_main(void)
{
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  /*Creating the binary semaphore  */
 EmployeeSignal= xSemaphoreCreateBinary();
 if(EmployeeSignal==NULL)
 {
	 printf("Failed to Create Binary Semaphore\r\n");
	 vSemaphoreDelete(EmployeeSignal);
 }

 xTaskCreate(BossThread, "Boss", 1024, NULL, 1, &BossTh);
 if(BossTh==NULL)
 {
	 printf("Failed to create the BossThread\r\n");
	 vTaskDelete(BossTh);
 }

 xTaskCreate(EmployeeThread, "employee", 1024, NULL, 2, &EmployeeTh);
 if(EmployeeTh==NULL)
 {
	 printf("Failed to create EmployeeThread\r\n");
	 vTaskDelete(EmployeeTh);
 }


}

