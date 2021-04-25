/* FreeRTOS Queue simple example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

/***********************************
 *FreeRTOS Queue Example1 
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/


TaskHandle_t Handler_1=NULL;
TaskHandle_t Handler_2=NULL;
QueueHandle_t QueueHandler=NULL;

void Sender(void)
{
	uint16_t i=0;
	for(;;)
	{
		if(xQueueSend(QueueHandler, &i, 1000)== pdTRUE)
		{
		     printf("Sent: %d\r\n", i);
		}
		else
		{
			printf("Failed to send!\r\n");
		}
        i++;
	    vTaskDelay(1000/portTICK_PERIOD_MS);
	}

}


void Receiver(void)
{
	uint16_t j=0;
	for(;;)
	{
	    if(xQueueReceive(QueueHandler, &j, portMAX_DELAY)== pdTRUE)
	    {
		    printf("Received: %d\r\n", j);
	    }
	    else
	    {
		    printf("Failed to Receive!\r\n");
	    }

	}
}


void app_main(void)
{
    BaseType_t xReturn =NULL;
    /* Creating the Queue having the size of int */
    QueueHandler = xQueueCreate(2, sizeof(int));
    if(QueueHandler==NULL)
    {
       printf("Failed to create the Queue!\r\n");
    }
	

	xReturn= xTaskCreate(Sender, "SenderThread", 2048, NULL, 1, &Handler_1);
	if(xReturn==NULL)
	{
		vTaskDelete(Handler_1);
	}

	xReturn= xTaskCreate(Receiver, "ReceiverThread", 2048, NULL, 2, &Handler_2);
	if(xReturn==NULL)
	{
		vTaskDelete(Handler_2);
	}

}

