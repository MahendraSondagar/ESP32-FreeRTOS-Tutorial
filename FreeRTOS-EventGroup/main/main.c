/* Event group 1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>`
#include <freertos/event_groups.h>
/***********************************
 *FreeRTOS Event group
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/

/* Task Global handler */
TaskHandle_t Task1Handler =NULL;
TaskHandle_t Task2Handler =NULL;
TaskHandle_t EventTaskHandler = NULL;

/* event group Global handler */
EventGroupHandle_t MyEvent =NULL;

/* Event group flags or bits to capture the desigred events*/
#define TX_EVENT       (1<<0)
#define RX_EVENT       (1<<1)
#define FRAME_ERROR    (1<<2)

void Task1(void *p)
{
    uint8_t Count =0;
    for(;;)
    {
        /* Generating the TX Event at every 3 sec */
        xEventGroupSetBits(MyEvent, TX_EVENT);  
        if(Count ++ >= 10)
        {
            /*Generating the TX Event with frame error at every 10 counts */
            xEventGroupSetBits(MyEvent, TX_EVENT | FRAME_ERROR);
            Count =0;
        } 
        vTaskDelay(3000/portTICK_PERIOD_MS);

    }
}

void Task2(void *p)
{
    for(;;)
    {
        /* Setting the RX Event at every 5 sec*/
        xEventGroupSetBits(MyEvent, RX_EVENT);
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}


void EventWaitTask(void *p)
{
    EventBits_t MyBits= NULL;
    for(;;)
    {
        MyBits = xEventGroupWaitBits(MyEvent, TX_EVENT | RX_EVENT | FRAME_ERROR,         // Wait for the perticular flag
                                        pdTRUE,                                          // Clear on exit (RST flag)
                                             pdFALSE,                                    // Wait for all bits False
                                                   portMAX_DELAY);                       //port MAX delay to wait 
        if(MyBits & TX_EVENT)
        {
            printf("Data Sent \r\n");
        }  

        if(MyBits & RX_EVENT)
        {
            printf("Data Received\r\n");
        }

        if(MyBits & FRAME_ERROR)
        {
            printf("FRAME Error detected!\r\n");
        }
    }
}

void app_main(void)
{
   BaseType_t xReturn = NULL;

   /* Creating the Event group */
   MyEvent = xEventGroupCreate();
   if(MyEvent== NULL)
   {
       printf("Failed to Create Event group!\r\n");
       vEventGroupDelete(MyEvent);
   }
   xReturn = xTaskCreate(Task1, "task1", 1024, NULL, 2, &Task1Handler);
   if(xReturn == NULL)
   {
       printf("Failed to create Task1\r\n");
       vTaskDelete(Task1Handler);
   }

   xReturn = xTaskCreate(Task2, "task2", 1024, NULL, 1, &Task2Handler);
   if(xReturn == NULL)
   {
       printf("Failed to create Task2\r\n");
       vTaskDelete(Task2Handler);
   }

   xReturn = xTaskCreate(EventWaitTask, "event", 1024, NULL, 3, &EventTaskHandler);
   if(xReturn ==NULL)
   {
       printf("Failed to creat Event Wait task\r\n");
       vTaskDelete(EventTaskHandler);
   }

}
