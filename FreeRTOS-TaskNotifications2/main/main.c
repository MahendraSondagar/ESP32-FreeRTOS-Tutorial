/* FreeRTOS TaskNotifications2 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t NotifierHandler =NULL;
TaskHandle_t ReceiverHandler =NULL;


/***********************************
 *FreeRTOS Task Notification 
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/

/* Notifier task to simply send the Notifications value to receiver task*/
/* Action required none */
void NotifierTask(void *p)
{
    for(;;)
    {
        if(ReceiverHandler !=NULL)
        {
            /* Taskhandler , ulValue (NotifyCount 32 bit), Action*/
            xTaskNotify(ReceiverHandler, 0, eNoAction);      /* ulValue = 0, Action = None */      
            vTaskDelay(250/portTICK_PERIOD_MS);
        }
        else
        {
          printf("Receiver task handler is null!\r\n");
        }
       
    }
}

void ReceiverTask(void *p)
{
    uint32_t NotiFyCount =0;
    for(;;)
    {
        /*ulBitsToClearOnEntry, ulBitsToClearOnExit, NotifyCount, portMAX_DELAY to wait*/
       if( xTaskNotifyWait(0,0, &NotiFyCount, portMAX_DELAY) ==pdTRUE)
       {
           printf("Received notifications count: %d\r\n", NotiFyCount);
       }
    }
}



void app_main(void)
{
    BaseType_t xReturn =NULL;
    printf(" ---- FreeRTOS TaskNotifications2 Example ---- \r\n");
    xReturn  = xTaskCreate(NotifierTask, "Notify", 2048, NULL, 2, &NotifierHandler);
    if(xReturn == NULL)
    {
        printf("Failed to Create NotifierTask\r\n");
        vTaskDelete(NotifierTask);
    }

    xReturn = xTaskCreate(ReceiverTask, "Receive", 2048, NULL, 1, &ReceiverHandler);
    if(xReturn== NULL)
    {
        printf("Failed to Create Receiver task\r\n");
        vTaskDelete(ReceiverHandler);
    }
}
