#include "boardconfig.h"
#include "boarddef.h"
#include "driver/gpio.h"

void board_config()
{
    /* Configure LED_1 */
    gpio_pad_select_gpio(LED1_PIN);
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);

    /* Configure LED_2 */
    gpio_pad_select_gpio(LED2_PIN);
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);

    /* Configure LED_3 */
    gpio_pad_select_gpio(LED3_PIN);
    gpio_set_direction(LED3_PIN, GPIO_MODE_OUTPUT);

}