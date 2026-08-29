#include <stdio.h>

#include "lwip/ip4_addr.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "ap_config.h"


esp_netif_t *ap_handle = 0;

wifi_config_t ap_config = {
	.ap = {
		.ssid = AP_SSID,
		.password = AP_PASSWORD,
		.ssid_len = AP_SSID_LEN,
		.channel = AP_CHANNEL,
		.authmode = AP_AUTH_MODE,
		.max_connection = AP_MAX_CONNECTION,
	},
};

void setup_ap();
void configure_wifi();
void configure_ip();
void err_halt(char*,...);

void app_main(void)
{
	setup_ap();

	while (1) {
	}
}

void 
setup_ap()
{
	configure_wifi();
	configure_ip();
}

void 
configure_wifi()
{
	wifi_init_config_t  init_config = WIFI_INIT_CONFIG_DEFAULT();

	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	
	ap_handle = esp_netif_create_default_wifi_ap();

	ESP_ERROR_CHECK(esp_wifi_init(&init_config));

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

	ESP_ERROR_CHECK(esp_wifi_start());
}

void 
configure_ip()
{
	esp_netif_ip_info_t			ip = {0};
	esp_netif_dhcp_status_t		dhcp_status = ESP_NETIF_DHCP_STARTED;

	ESP_ERROR_CHECK(esp_netif_dhcps_stop(ap_handle));
	while (dhcp_status != ESP_NETIF_DHCP_STOPPED) 
		ESP_ERROR_CHECK(esp_netif_dhcps_get_status(ap_handle, &dhcp_status));

	if ((ip.ip.addr = ipaddr_addr(AP_IP_ADDRESS)) == IPADDR_NONE)
		err_halt("failed to convert ip address: %s", AP_IP_ADDRESS);

	if ((ip.netmask.addr = ipaddr_addr(AP_IP_NETMASK)) == IPADDR_NONE)
		err_halt("failed to convert netmask: %s", AP_IP_NETMASK);

	ESP_ERROR_CHECK(esp_netif_set_ip_info(ap_handle, &ip));
}

void 
err_halt(char *fmt,...)
{
	while (1)
		;
}
