
/* FreeRTOS Mutex Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

/***********************************
 *FreeRTOS First Mutex Ex.
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/

TaskHandle_t th1=NULL;
TaskHandle_t th2=NULL;
SemaphoreHandle_t GlobalHandler=NULL;

/* Precious resource function which might be the i2c/ spi or UART bus*/
void Precious_Resource(void)
{

}

void Thread_1(void *p)
{
   for(;;)
   {
	   if(xSemaphoreTake(GlobalHandler, 250)==pdTRUE)
	   {
		   printf("Thread1 got the resource\r\n");
		   Precious_Resource();
		   xSemaphoreGive(GlobalHandler);
	   }
	   vTaskDelay(250/portTICK_PERIOD_MS);
   }

}

void Thread_2(void *p)
{
	for(;;)
	{
		if(xSemaphoreTake(GlobalHandler, 250)==pdTRUE)
		{
			printf("Thread2 got the resource\r\n");
			Precious_Resource();
			xSemaphoreGive(GlobalHandler);
		}
		vTaskDelay(250/portTICK_PERIOD_MS);
	}

}


void app_main(void)
{
	/* Creating the Mutex */
 GlobalHandler= xSemaphoreCreateMutex();
 if(GlobalHandler==NULL)
 {
	 printf("Failed to Create Mutex\r\n");
 }

 xTaskCreate(Thread_1, "Thread1", 1024, NULL, 1, &th1);
 if(th1==NULL)
 {
	 printf("Failed to create Task1\r\n");
	 vTaskDelete(th1);
 }

 xTaskCreate(Thread_2, "Thread2", 1024, NULL, 2, &th2);
 if(th2==NULL)
 {
	 printf("Failed to create task2\r\n");
	 vTaskDelete(th2);
 }


}

