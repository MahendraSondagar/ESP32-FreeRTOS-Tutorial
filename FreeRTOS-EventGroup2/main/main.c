/* Event group from ISR Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include "driver/gpio.h"

/***********************************
 *FreeRTOS Event group from ISR 
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
/* switch flag to send from ISR*/
#define SWITCH_FLAG (1<<0)
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0



TaskHandle_t        EventTaskHandler= NULL;
EventGroupHandle_t  MyEvent         = NULL;


static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    BaseType_t HighPriorityTaskWoken =NULL;
    /* Sending the event bits from the ISR */
    xEventGroupSetBitsFromISR(MyEvent, SWITCH_FLAG, &HighPriorityTaskWoken);
}

void EventWaitThread(void *p)
{
    EventBits_t MyBits =NULL;
  for(;;)
  {
      MyBits= xEventGroupWaitBits(MyEvent, SWITCH_FLAG, pdTRUE, pdFALSE, portMAX_DELAY);
      if(MyBits & SWITCH_FLAG)
      {
          printf("Switch has been pressed!\r\n");
      }
  }
}

void app_main(void)
{
   printf(" --- FreeRTOS Event group from ISR ---\r\n");
    BaseType_t xReturn = NULL;   /* Creating the event group */
    MyEvent = xEventGroupCreate();

   if(MyEvent == NULL)
   {
       printf("Failed to create Event group!\r\n");
       vEventGroupDelete(MyEvent);
   }

   
   MyEvent = xTaskCreate(EventWaitThread, "event", 2048, NULL, 1, &EventTaskHandler);
   if(MyEvent == NULL)
   {
       printf("Failed to create Task\r\n");
       vTaskDelete(EventTaskHandler);
   }

    gpio_config_t io_conf;
   //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);
  

    /* install gpio isr service */
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());


}
