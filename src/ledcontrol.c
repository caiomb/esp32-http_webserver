#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "boarddef.h"
#include "ledcontrol.h"

static void control_led1_task(void *pvParameters);
static void control_led2_task(void *pvParameters);
static void control_led3_task(void *pvParameters);

SemaphoreHandle_t led1_mutex;
SemaphoreHandle_t led2_mutex;
SemaphoreHandle_t led3_mutex;

typedef enum {OFF, ON, BLINK} led_state_t;
typedef struct _led {
    led_state_t state;
    int delay;  
} led_t;

static led_t led1;
static led_t led2;
static led_t led3;

void init_leds_task()
{
    led1_mutex = xSemaphoreCreateMutex();
    if(led1_mutex == NULL)
    {
        printf("Falha ao criar Mutex! \n");
    }

    led2_mutex = xSemaphoreCreateMutex();
    if(led2_mutex == NULL)
    {
        printf("Falha ao criar Mutex! \n");
    }

    led3_mutex = xSemaphoreCreateMutex();
    if(led3_mutex == NULL)
    {
        printf("Falha ao criar Mutex! \n");
    }

    led1.state = OFF;
    led2.state = OFF;
    led3.state = OFF;

    xTaskCreate(&control_led1_task, "Control Led 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(&control_led2_task, "Control Led 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(&control_led3_task, "Control Led 3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
}

static int get_state_from_string(char *led_state)
{
    if (strcmp(led_state, "OFF") == 0)
    {
        return OFF;
    } 
    else if(strcmp(led_state, "ON") == 0)
    {
        return ON;
    }
    else if(strcmp(led_state, "BLINK") == 0)
    {
        return BLINK;
    }
    return OFF;  
}

void set_led_state(int led, char* led_state, int delay) 
{
    int state = get_state_from_string(led_state);
    switch (led) 
    {
        case 1:
            xSemaphoreTake(led1_mutex, portMAX_DELAY);
            led1.state = state;
            led1.delay = delay;
            xSemaphoreGive(led1_mutex);
            break;
        case 2:
            xSemaphoreTake(led2_mutex, portMAX_DELAY);
            led2.state = state;
            led2.delay = delay;
            xSemaphoreGive(led2_mutex);
            break;
        case 3:
            xSemaphoreTake(led3_mutex, portMAX_DELAY);
            led3.state = state;
            led3.delay = delay;
            xSemaphoreGive(led3_mutex);
            break;
        default:
            break;
    }
}

static void control_led1_task(void *pvParameters)
{
    bool level = 0;
    while (true)
    {
        xSemaphoreTake(led1_mutex, portMAX_DELAY);
        switch (led1.state)
        {
            case ON:
                gpio_set_level(LED1_PIN, ON);
                vTaskDelay( 1000 / portTICK_PERIOD_MS );
                break;
            case OFF:
                gpio_set_level(LED1_PIN, OFF);
                vTaskDelay( 1000 / portTICK_PERIOD_MS );
                break;
            case BLINK:
                gpio_set_level(LED1_PIN, level);
                level = !level;
                vTaskDelay( led1.delay / portTICK_PERIOD_MS );
                break;    
        };
        xSemaphoreGive(led1_mutex);
    }
}

static void control_led2_task(void *pvParameters)
{
    bool level = 0;
    while (true)
    {
        xSemaphoreTake(led2_mutex, portMAX_DELAY);
        switch (led2.state)
        {
            case ON:
                gpio_set_level(LED2_PIN, ON);
                vTaskDelay( 1000 / portTICK_PERIOD_MS );
                break;
            case OFF:
                gpio_set_level(LED2_PIN, OFF);
                vTaskDelay( 1000 / portTICK_PERIOD_MS );
                break;
            case BLINK:
                gpio_set_level(LED2_PIN, level);
                level = !level;
                vTaskDelay( led2.delay / portTICK_PERIOD_MS );
                break;    
        };
        xSemaphoreGive(led2_mutex);
    }
}

static void control_led3_task(void *pvParameters)
{
    bool level = 0;
    while (true)
    {
        xSemaphoreTake(led3_mutex, portMAX_DELAY);
        switch (led3.state)
        {
            case ON:
                gpio_set_level(LED3_PIN, ON);
                vTaskDelay( 1000 / portTICK_PERIOD_MS );
                break;
            case OFF:
                gpio_set_level(LED3_PIN, OFF);
                vTaskDelay( 1000 / portTICK_PERIOD_MS );
                break;
            case BLINK:
                gpio_set_level(LED3_PIN, level);
                level = !level;
                vTaskDelay( led3.delay / portTICK_PERIOD_MS );
                break;    
        };
        xSemaphoreGive(led3_mutex);
    }
}