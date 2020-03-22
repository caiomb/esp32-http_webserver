#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"
#include "lwip/err.h"
#include "server.h"
#include "ledcontrol.h"

#define EXAMPLE_ESP_WIFI_SSID      "CAIO_ESP32"
#define EXAMPLE_ESP_WIFI_PASS      "12345678"
#define EXAMPLE_MAX_STA_CONN       2

static const char *TAG = "wifi softAP";
static EventGroupHandle_t s_wifi_event_group;
const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const static char http_index_html_server[] = "\
<html>\
<body>\
<center><h1>Projeto 2 - Webserver</h1></center>\
<center><h1>Arquitetura de redes de comunicação</h1></center>\
<table>\
<tr>\
<td>\
<form>\
<button name=\"LED1\" button style=\"color:green\" value=\"ON\"\
type=\"submit\"> LED1 ON </button>\
<button name=\"LED1\" button style=\"color:red\" value=\"OFF\"\
type=\"submit\"> LED1 OFF </button>\
</form>\
</td>\
<td>\
<form>\
<button type=\"submit\">BLINKY</button>\
<label for=\"delay\">Delay (ms):</label>\
<input type=\"text\" name=\"BLINK1\" value=\"0\">\
</form>\
</td>\
</tr>\
<tr>\
<td>\
<form>\
<button name=\"LED2\" button style=\"color:green\" value=\"ON\"\
type=\"submit\"> LED2 ON </button>\
<button name=\"LED2\" button style=\"color:red\" value=\"OFF\"\
type=\"submit\"> LED2 OFF </button>\
</form>\
</td>\
<td>\
<form>\
<button type=\"submit\">BLINKY</button>\
<label for=\"delay\">Delay (ms):</label>\
<input type=\"text\" name=\"BLINK2\" value=\"0\">\
</form>\
</td>\
</tr>\
<tr>\
<td>\
<form>\
<button name=\"LED3\" button style=\"color:green\" value=\"ON\"\
type=\"submit\"> LED3 ON </button>\
<button name=\"LED3\" button style=\"color:red\" value=\"OFF\"\
type=\"submit\"> LED3 OFF </button>\
</form>\
</td>\
<td>\
<form>\
<button type=\"submit\">BLINKY</button>\
<label for=\"delay\">Delay (ms):</label>\
<input type=\"text\" name=\"BLINK3\" value=\"0\">\
</form>\
</td>\
</tr>\
</table>\
</body>\
</html>";

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id)
    {
        case SYSTEM_EVENT_AP_STACONNECTED:
            ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
                     MAC2STR(event->event_info.sta_connected.mac),
                     event->event_info.sta_connected.aid);
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
                     MAC2STR(event->event_info.sta_disconnected.mac),
                     event->event_info.sta_disconnected.aid);
            break;
        default:
            break;
    }
    return ESP_OK;
}

void wifi_init_softap()
{
    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) 
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished.SSID:%s password:%s",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}

static bool http_query_key_value(char *qry_str, const char *key, char *val)
{
    if (qry_str == NULL || key == NULL || val == NULL) 
    {
        return false;
    }

    if(strstr(qry_str, "favicon"))
    {
        return false;
    }

    char *request_line = strtok(qry_str, "\n");
    if(request_line)
    {
        char* aux_key = strstr(request_line, key);
        if(aux_key)
        {
            aux_key = strtok(aux_key, " ");
            aux_key = strtok(aux_key, "=");
            char* aux_value = strtok(NULL, "=");
            if(aux_value)
            {
                strcpy(val, aux_value);
                return true;
            }
        }
    }
    return false;
}

static void http_server_netconn_serve(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *buf;
    u16_t buflen;
    err_t err;
    err = netconn_recv(conn, &inbuf);
    if (err == ERR_OK) 
    {
        netbuf_data(inbuf, (void**)&buf, &buflen);
        buf[buflen] = '\0';
        char value[32] = "\0";

        /* Get value of expected key from query string */
        if (http_query_key_value(buf, "LED1", value)) 
        {
            ESP_LOGI(TAG, "Found URL query parameter => LED1=%s\n", value);
            set_led_state(1, value, 0);
        }
        else if (http_query_key_value(buf, "LED2", value)) 
        {
            ESP_LOGI(TAG, "Found URL query parameter => LED2=%s\n", value);
            set_led_state(2, value, 0);
        }
        else if (http_query_key_value(buf, "LED3", value)) 
        {
            ESP_LOGI(TAG, "Found URL query parameter => LED3=%s\n", value);
            set_led_state(3, value, 0);
        }
        else if (http_query_key_value(buf, "BLINK1", value)) 
        {
            ESP_LOGI(TAG, "Found URL query parameter => BLINK1=%s\n", value);
            set_led_state(1, "BLINK", atoi(value));
        }
        else if (http_query_key_value(buf, "BLINK2", value)) 
        {
            ESP_LOGI(TAG, "Found URL query parameter => BLINK2=%s\n", value);
            set_led_state(2, "BLINK", atoi(value));
        }
        else if (http_query_key_value(buf, "BLINK3", value)) 
        {
            ESP_LOGI(TAG, "Found URL query parameter => BLINK3=%s\n", value);
            set_led_state(3, "BLINK", atoi(value));
        }

        netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
        netconn_write(conn, http_index_html_server, sizeof(http_index_html_server)-1, NETCONN_NOCOPY);
    }
    netconn_close(conn);
    netbuf_delete(inbuf);
}

static void http_server(void *pvkeys)
{
    struct netconn *conn, *newconn;
    err_t err;
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 80);
    netconn_listen(conn);

    do {
        err = netconn_accept(conn, &newconn);
        if (err == ERR_OK) 
        {
            http_server_netconn_serve(newconn);
            netconn_delete(newconn);
        }
    } while(err == ERR_OK);

    netconn_close(conn);
    netconn_delete(conn);
}

void http_server_setup()
{
    wifi_init_softap();
    xTaskCreate(&http_server, "http_server", 2048, NULL, 5, NULL);
}