/* FreeRTOS task notification basic test

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/* FreeRTOS Task Notification 
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t  NotifierTaskHandler = NULL;
TaskHandle_t  ReceiverTaskHandler = NULL;



/***********************************
 *FreeRTOS Task notification
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/
 
/* Notifier task */
void NotifierTask(void *p)
{
    for(;;)
    {     
        /* Sending the Notifications to task2 by every 1 sec*/
        if(ReceiverTaskHandler!=NULL)
        {
            /*Notify to task with handler */
            xTaskNotifyGive(ReceiverTaskHandler);
            vTaskDelay(1000/portTICK_PERIOD_MS);
        }
        else
        {
            printf("Null Task2Handler!\r\n");
        }
          
    }
}


void ReceiverTask(void *p)
{
 
 uint32_t NotifyCounts=0;
 for(;;)
 {
     /*
     Wait for the notifications for the portMAX_Delay blocking time
     pdTRUE means reseting the Notifications every time
     */
    NotifyCounts= ulTaskNotifyTake(pdTRUE, portMAX_DELAY);   
    if(NotifyCounts>0)
    {
       for (;;)
       {
           printf("Received Notifiocation: %d\r\n", NotifyCounts);
       }
    }
 } 
}

void app_main(void)
{
   printf("---- FreeRTOS TaskNotificatios Example ---- \r\n");
   BaseType_t xReturned;
   xReturned= xTaskCreate(NotifierTask, "task1", 2048, NULL, 2, &NotifierTaskHandler);
   if(xReturned==NULL)
    {
        printf("Failed to Create task1\r\n");
        vTaskDelete(NotifierTaskHandler);
    }


    xReturned= xTaskCreate(ReceiverTask, "task2", 2048, NULL, 1, &ReceiverTaskHandler);
    if(xReturned==NULL)
    {
        printf("Failed to Create task2\r\n");
        vTaskDelete(ReceiverTaskHandler);
    }
}
