/* FreeRTOS Counting semaphore Example with ISR 

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "driver/gpio.h"
#include "sdkconfig.h"

/***********************************
 *FreeRTOS Counting Semaphore Ex.
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/

/*user switch io */
#define USER_SWITCH CONFIG_USER_SWITCH
#define GPIO_INPUT_PIN_SEL  (1ULL<< USER_SWITCH)
#define ESP_INTR_FLAG_DEFAULT 0

TaskHandle_t CntHandler =NULL;
SemaphoreHandle_t MySem =NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
/* The event has occurred. Use the semaphore to unblock the task so the task can
process the event. */
 static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 xSemaphoreGiveFromISR(MySem, &xHigherPriorityTaskWoken);

  /* Clear the interrupt here. */
/* Now that the task has been unblocked, a context switch should be performed
if xHigherPriorityTaskWoken is equal to pdTRUE. NOTE: The syntax required to perform a
context switch from an ISR varies from port to port and from compiler to compiler. Check
the web documentation and examples for the port being used to find the syntax required for
your application. */

}


void CounterThread(void *p)
{
    for(;;)
    {
        if(xSemaphoreTake(MySem, portMAX_DELAY)==pdTRUE)
        {
           printf("Event counter: [%d]\r\n",uxSemaphoreGetCount(MySem));
        }
        vTaskDelay(250/portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
        printf(" --- FreeRTOS Counting Semaphore as an event counter ---\r\n");
        MySem = xSemaphoreCreateCounting(10, 0);
        if(MySem == NULL)
        {
            printf("Failed to create Counting Semaphore\r\n");
            vSemaphoreDelete(MySem);
        }
    
   	    gpio_config_t io_conf; 
      	//interrupt of rising edge
         io_conf.intr_type = GPIO_INTR_POSEDGE;
        //bit mask of the pins, use GPIO4 here
        io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
        //set as input mode
        io_conf.mode = GPIO_MODE_INPUT;
        //enable pull-up mode
        io_conf.pull_up_en = 1;
        gpio_config(&io_conf);

        //change gpio intrrupt type for one pin
        gpio_set_intr_type(GPIO_INPUT_PIN_SEL, GPIO_INTR_ANYEDGE);
        //set as input mode
    if(xTaskCreate(CounterThread, "Count",2048, NULL, 1, &CntHandler)!=pdTRUE)
    {
        printf("Failed to Create the Eventconter Thread\r\n");
        vTaskDelete(CntHandler);
    }

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(USER_SWITCH, gpio_isr_handler, (void*) USER_SWITCH);
   

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(USER_SWITCH);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(USER_SWITCH, gpio_isr_handler, (void*) USER_SWITCH);
    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
}
