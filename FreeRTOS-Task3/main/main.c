/* FreeRTOS Task example3

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/***********************************
 *FreeRTOS Task parameter
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/

/* Task global handler */
TaskHandle_t th1=NULL;
TaskHandle_t th2=NULL;
TaskHandle_t th3=NULL;

void Thread_3(void)
{
	uint32_t TaskCount=0;
	while(1)
	{
		printf("Thread_3 is Running!\r\n");
		vTaskDelay(250/portTICK_PERIOD_MS);
		if(++TaskCount==25)
		{
			printf("Resuming all 2 task!\r\n");
			vTaskResume(th1);
			vTaskResume(th2);
		}

		if(++TaskCount==50)
		{
			printf("Deleting all Task\r\n");
			vTaskDelete(th1);
			vTaskDelete(th2);
			vTaskDelete(th3);
		}


	}

}

void Thread_1(void)
{
	uint32_t TaskCount=0;
	while(1)
	{
		printf("Thread_1 is Running!\r\n");
		vTaskDelay(250/portTICK_PERIOD_MS);
		if(++TaskCount==25)
		{
			printf("Suspending--->Task1\r\n");
			vTaskSuspend(th1);
		}

	}

}


void Thread_2(void)
{
	uint32_t TaskCount=0;
	while(1)
	{
		printf("Thread_2 is Running!\r\n");
		vTaskDelay(250/portTICK_PERIOD_MS);
		if(++TaskCount==50)
		{
			printf("Creating the new task3\r\n");
			xTaskCreate(Thread_3, "thread3", 1024, 3, NULL, &th3);
			if(th3==NULL)
			{
				printf("Failed to create 3rd task!\r\n");
				vTaskDelete(th3);
			}

			printf("Suspending--->Task2\r\n");
			vTaskSuspend(th2);

		}

	}

}


void app_main(void)
{
    BaseType_t xReturn = NULL;
	xReturn= xTaskCreate(Thread_1, "thread1", 1024, 1, NULL, &th1);
	if(xReturn == NULL)
	{
		printf("Failed to create First task!\r\n");
		vTaskDelete(th1);
	}

	xReturn= xTaskCreate(Thread_2, "thread2", 1024,2, NULL, &th2);
	if(xReturn == NULL)
	{
		printf("Failed to create 2nd task!\r\n");
		vTaskDelete(th2);
	}
}

