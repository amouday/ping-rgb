#include <stdio.h>
#include <stdarg.h>

#include "lwip/ip4_addr.h"
#include "lwip/sockets.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "ap_config.h"
#include "rgb_config.h"
#include "rgb.h"
#include "utils.h"

#define BUFSIZE				512

enum {
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE,
	INDIGO,
	VIOLET,
	NUM_COLORS,
};

color_t rainbow[NUM_COLORS] = {
	[RED] 		= { .red = 0x3fff, .green = 0x0, .blue = 0x0 },
	[ORANGE] 	= { .red = 0x3fff, .green = 0x666, .blue = 0x0 },
	[YELLOW] 	= { .red = 0x2ccc, .green = 0x1332, .blue = 0x0 },
	[GREEN] 	= { .red = 0x0, .green = 0x3fff, .blue = 0x0 },
	[BLUE] 		= { .red = 0x0, .green = 0x0, .blue = 0x3fff },
	[INDIGO] 	= { .red = 0x1332, .green = 0x0, .blue = 0x3fff },
	[VIOLET] 	= { .red = 0x3fff, .green = 0x3e8, .blue = 0x3fff },
};

wifi_config_t ap_config = {
	.ap = {
		.ssid 			= AP_SSID,
		.password 		= AP_PASSWORD,
		.ssid_len 		= AP_SSID_LEN,
		.channel 		= AP_CHANNEL,
		.authmode 		= AP_AUTH_MODE,
		.max_connection = AP_MAX_CONNECTION,
	},
};

esp_netif_t *ap_handle = 0;
unsigned int color_index = RED;

void ap_init();
void configure_wifi();
void configure_ip();
void configure_dhcps();

void handle_packet(uint8_t *, size_t);
void dump_packet(uint8_t *, size_t);
void packet_test();

void color_test();
char *rainbow_to_str(unsigned int);

void 
app_main(void)
{
	int			sockfd;
	uint8_t		buf[BUFSIZE];
	ssize_t		recvlen;

	ap_init();
	rgb_init();

	if ((sockfd = lwip_socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		err_halt("lwip_socket failed");

	while (1) {
		if ((recvlen = lwip_recv(sockfd, buf, BUFSIZE, 0)) == -1) {
			ESP_LOGE("recv", "recv failed");
			continue;
		}

		handle_packet(buf, recvlen);
	}
}

void
handle_packet(uint8_t *packet, size_t len)
{
	dump_packet(packet, len);

	if (color_index == NUM_COLORS)
		color_index = RED;

	printf("[+] Setting color to: '%s'\n", rainbow_to_str(color_index));
	rgb_set_color(rainbow[color_index++]);

	puts("");
}

void
dump_packet(uint8_t *packet, size_t len)
{
	char		src_addr[INET_ADDRSTRLEN + 1];

	extract_src_addr(src_addr, packet);
	src_addr[INET_ADDRSTRLEN] = '\0';
	printf("[+] Recived ICMP packet from: '%s'\n", src_addr);

	hexdump(packet, len);

	puts("");
}

void 
ap_init()
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

	ESP_ERROR_CHECK(esp_netif_dhcps_start(ap_handle));
}


void 
color_test()
{
	while (1) {
		for (int i = 0; i < NUM_COLORS; i++) {
			rgb_set_color(rainbow[i]);
			vTaskDelay(500 / portTICK_PERIOD_MS);
		}
	}
}

void 
packet_test()
{
	int			sockfd;
	uint8_t		buf[BUFSIZE];
	ssize_t		recvlen;

	if ((sockfd = lwip_socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		err_halt("lwip_socket failed");

	while (1) {
		if ((recvlen = lwip_recv(sockfd, buf, BUFSIZE, 0)) == -1) {
			ESP_LOGE("recv", "recv failed");
			continue;
		}

		dump_packet(buf, recvlen);
	}
}

char *
rainbow_to_str(unsigned int color)
{
	switch (color) {
	case RED:			return "Red";
	case ORANGE:		return "Orange";
	case YELLOW:		return "Yellow";
	case GREEN:			return "Green";
	case BLUE:			return "Blue";
	case INDIGO:		return "Indigo";
	case VIOLET:		return "Violet";
	default:			return "Unknown";
	}
}
