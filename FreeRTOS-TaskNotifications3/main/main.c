
/* FreeRTOS TaskNotifications3  Example as a light weight eventgroup
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

/* WiFi various state */
typedef enum
{
  WiFi_INIT            =            (1<<0),
  WiFi_CREDENTIAL_SET  =            (1<<1),
  WiFi_GOT_IP          =            (1<<2),
  WiFi_SOCKET_CONNECT  =            (1<<3),
  WiFi_DISCONNECTED    =            (1<<4),
}WiFiNotify;


/* Notifier task to simply send the Notifications value to receiver task*/
/* Action required: eSetValueWithOverwrite */
/* TaskNotifier can be also used as light weight Eventgroup*/
void NotifierTask(void *p)
{
    for(;;)
    {
        if(ReceiverHandler !=NULL)
        {
            /*Setvalue over write alows to overwrite the each flahs without default Oring behaviour*/
            xTaskNotify(ReceiverHandler, WiFi_INIT, eSetValueWithOverwrite);                        
            vTaskDelay(1000/portTICK_PERIOD_MS);

            xTaskNotify(ReceiverHandler, WiFi_CREDENTIAL_SET, eSetValueWithoutOverwrite);      
            vTaskDelay(1000/portTICK_PERIOD_MS);

            xTaskNotify(ReceiverHandler, WiFi_GOT_IP, eSetValueWithoutOverwrite);              
            vTaskDelay(1000/portTICK_PERIOD_MS);

            xTaskNotify(ReceiverHandler, WiFi_SOCKET_CONNECT, eSetValueWithoutOverwrite);      
            vTaskDelay(1000/portTICK_PERIOD_MS);

            xTaskNotify(ReceiverHandler, WiFi_DISCONNECTED, eSetValueWithoutOverwrite);     
            vTaskDelay(1000/portTICK_RATE_MS);    
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
        /* ulBitsToClearOnEntry, ulBitsToClearOnExit, NotifyCount, portMAX_DELAY to wait*/
       if( xTaskNotifyWait(0,0, &NotiFyCount, portMAX_DELAY) ==pdTRUE)
       {
           switch (NotiFyCount)
           {
           case WiFi_INIT  :
                printf("WiFi has been init \r\n");
               break;

            case WiFi_CREDENTIAL_SET:
                printf("WiFi credential set\r\n");
               break;

            case WiFi_GOT_IP:
                printf("WiFi Got the IP\r\n");
               break;

            case WiFi_SOCKET_CONNECT:
                printf("WiFi Socket Connect set\r\n");
               break;

            case WiFi_DISCONNECTED:
                printf("WiFi Disconencted\r\n");
               break;
           
           default:
               break;
           }
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