/* FreeRTOS Autoreload timer example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

/***********************************
 *FreeRTOS Auto reload Timer
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/

/* Global FreeRTOS Timer handler */
TimerHandle_t MyTimer =NULL;

/* Timer Callback function to perform the Task*/
void MyTimerCallback(void *p)
{
  printf("Timer running...\r\n");  
}


void app_main(void)
{
    printf(" --- FreeRTOS Auto-reload timer example --- \r\n");
    /*
    Creating the auto-reload timer
    pdTRUE indicate the auto reload timer
    pdFALSE indicate the single shoot timer
    */
    MyTimer = xTimerCreate((const char *)"Timer",                   /* Timer debug string*/
                           pdMS_TO_TICKS(100),                      /* ms to TICK convertor macro*/
                           pdTRUE,                                  /* Auto-reload timer*/
                           (void *)0,MyTimerCallback);              /* Callback function */
    if(MyTimer == NULL)
    {
        printf("Failed to Create Timer\r\n");                  
        xTimerDelete(MyTimer, 0);
    }
    else
    {
       if(xTimerStart(MyTimer, 0)== pdTRUE)                         /* Start the timer if created!*/
       {
           printf("Timer Started\r\n");
       }
    }
}
