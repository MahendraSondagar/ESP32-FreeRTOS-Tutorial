#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

TaskHandle_t th1= NULL;
TaskHandle_t th2=NULL;
SemaphoreHandle_t MySem= NULL;


void Task1(void *p)
{
	for(;;)
	{
	    if(xSemaphoreTakeRecursive(MySem,500))
	    {
		    printf("Task1 got the resources\r\n");
		    xSemaphoreGiveRecursive(MySem);
	    }
	    vTaskDelay(250/portTICK_PERIOD_MS);
	}
}


void Task2(void *p)
{
	for(;;)
	{
		if(xSemaphoreTakeRecursive(MySem, 500))
		{
			printf("Task2 got the resources\r\n");
			xSemaphoreGiveRecursive(MySem);
		}
		vTaskDelay(250/portTICK_PERIOD_MS);
	}
}

void app_main(void)
{
  MySem= xSemaphoreCreateRecursiveMutex();
  if(MySem==NULL)
  {
    vSemaphoreDelete(MySem);		 
  }

  xTaskCreate(Task1, "task1", 1024, NULL, 1, &th1);
  if(th1== NULL)
  {
	  printf("Failed to create Task1\r\n");
	  vTaskDelete(th1);
  }

  xTaskCreate(Task2, "task2", 1024, NULL, 2, &th2);
  if(th2 ==NULL)
  {
	  printf("Failed to create task2\r\n");
	  vTaskDelete(th2);
  }


}
