#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

TaskHandle_t Handler_1=NULL;
TaskHandle_t Handler_2=NULL;

QueueHandle_t QueueGlobalHandler=NULL;

typedef struct _QueueData
{
	char *bytes;
	uint16_t QUEUE_ID;

}QueueData;

#define QUEUE_LENGHT 12
#define QUEUE_SIZE  sizeof(QueueData)

void SenderThread(void)
{
	QueueData SendData;
	SendData.bytes= "Hello World";
	SendData.QUEUE_ID= 0X01;
	for(;;)
	{
	    if(!xQueueSend(QueueGlobalHandler, &SendData, 1000))
	    {
		    printf("Failed to send!\r\n");
	    }
           vTaskDelay(1000/portTICK_PERIOD_MS);
	}

}



void ReceiverThread(void)
{
	QueueData ReceiverData;
	for(;;)
	{
		if(xQueueReceive(QueueGlobalHandler, &ReceiverData, portMAX_DELAY))
		{
			printf("Queue id[0x%02x], Received: %s\r\n", ReceiverData.QUEUE_ID, ReceiverData.bytes);
		}
		else
		{
			printf("Failed to Receive Queue!\r\n");
		}

	}

}



void app_main(void)
{

 printf("--- FreeRTOS Queue Demo With struct ----\r\n");
 QueueGlobalHandler = xQueueCreate(QUEUE_LENGHT , QUEUE_SIZE);
 if(QueueGlobalHandler==NULL)
 {
	 printf("Failed to create Queue\r\n");
 }

 xTaskCreate(SenderThread, "send", 2014, NULL, 1, &Handler_1);
 if(Handler_1==NULL)
 {
	 vTaskDelete(Handler_1);
 }

 xTaskCreate(ReceiverThread, "Receive", 2048, NULL, 2, &Handler_2);
 if(Handler_2==NULL)
 {
	 vTaskDelete(Handler_2);
 }


}

