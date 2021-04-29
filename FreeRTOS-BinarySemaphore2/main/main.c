#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/

/***********************************
 *FreeRTOS Binary Semaphore Ex
 *Author: Mahendra Sondagar
 *mahendrasondagar08@gmail.com
 * ********************************/

#define BLINK_GPIO  CONFIG_BLINK_GPIO
#define USER_SWITCH CONFIG_USER_SWITCH

#define GPIO_INPUT_PIN_SEL  (1ULL<<USER_SWITCH)
#define ESP_INTR_FLAG_DEFAULT 0


TaskHandle_t ProcessHandler=NULL;
SemaphoreHandle_t ProcessSignal=NULL;

static void Toggle_LED(void)
{
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
}


static void IRAM_ATTR gpio_isr_handler(void* arg)
{
/* The event has occurred. Use the semaphore to unblock the task so the task can
process the event. */
 static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(ProcessSignal, &xHigherPriorityTaskWoken);

  /* Clear the interrupt here. */
/* Now that the task has been unblocked, a context switch should be performed
if xHigherPriorityTaskWoken is equal to pdTRUE. NOTE: The syntax required to perform a
context switch from an ISR varies from port to port and from compiler to compiler. Check
the web documentation and examples for the port being used to find the syntax required for
your application. */

}
void ProcessThread(void *p)
{
	for(;;)
	{
		/* Wait for the binary semaphore from ISR*/
		if(xSemaphoreTake(ProcessSignal, portMAX_DELAY))
		{
			printf("Performing the LED toggle\r\n");
			Toggle_LED();
		}
	}

}


void app_main(void)
{
	 printf("FreeRTOS Binary Semaphore from ISR demo\r\n");
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
	gpio_reset_pin(BLINK_GPIO);
    	/* Set the GPIO as a push/pull output */
    	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

	ProcessSignal= xSemaphoreCreateBinary();
	if(ProcessSignal==NULL)
	{
		printf("Failed to create Binary Semaphore!\r\n");
		vSemaphoreDelete(ProcessHandler);
	}

	xTaskCreate(ProcessThread, "Process", 2048, NULL, 1, &ProcessHandler);
	if(ProcessHandler==NULL)
	{
		printf("Filed to Create process Thread!\r\n");
		vTaskDelete(ProcessHandler);
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

