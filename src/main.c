#include "esp_err.h"
#include "nvs_flash.h"
#include "server.h"
#include "boardconfig.h"
#include "ledcontrol.h"

void app_main()
{
    /* Inicialização do sistema */
    ESP_ERROR_CHECK(nvs_flash_init());
    board_config();
    init_leds_task();
    http_server_setup();
}