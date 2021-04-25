#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/***********************************
 *FreeRTOS Task parameter
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/

	/*FreeRTOS Task handler */
TaskHandle_t th1=NULL;
TaskHandle_t th2=NULL;

void Thread1(void *p)
{
	char *str1 = (char *)p;
	for(;;)
	{
		printf("Thread1: %s\r\n", str1);
		vTaskDelay(250/portTICK_PERIOD_MS);
	}

}

void Thread2(void *p)
{
	char *str2 = (char *)p;
	for(;;)
	{
		printf("Thread2: %s\r\n", str2);
		vTaskDelay(250/portTICK_PERIOD_MS);
	}

}



void app_main(void)
{
	/* Creating the FreeRTOS Task*/
	BaseType_t xReturn= NULL;

	xReturn= xTaskCreate(Thread1, "thread1", 1500, (char *)"Hello world\r\n",    1, &th1);
	if(xReturn==NULL)
	{
		printf("Fail to start Thread1!\r\n");
		vTaskDelete(th1);
	}

	xReturn=xTaskCreate(Thread2, "thread2", 1500, (char *)"Hello Universe\r\n", 2, &th2);
	if(xReturn==NULL)
	{
		printf("Fail to start Thread2\r\n");
		vTaskDelete(th2);
	}
}

