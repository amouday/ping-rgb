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

#define BUFSIZE				512
#define HEXDUMP_LINELEN		16
#define SRC_ADDR_OFFSET		12

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
void rgb_init();
void configure_wifi();
void configure_ip();
void err_halt(char*,...);
void packet_test();
void hexdump(uint8_t *, size_t);
void hexdump_line(uint8_t *, size_t);
void handle_packet(uint8_t *, size_t);
int extract_src_addr(char *, uint8_t *);

void 
app_main(void)
{
	setup_ap();
	rgb_init();

	ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, RGB_PWM_CHAN_RED, 8192));
	ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, RGB_PWM_CHAN_RED));

	ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, RGB_PWM_CHAN_GREEN, 500));
	ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, RGB_PWM_CHAN_GREEN));

	ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, RGB_PWM_CHAN_BLUE, 5000));
	ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, RGB_PWM_CHAN_BLUE));

	while (1) {
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

		handle_packet(buf, recvlen);
	}
}

void 
handle_packet(uint8_t *packet, size_t len)
{
	char		src_addr[INET_ADDRSTRLEN + 1];

	extract_src_addr(src_addr, packet);
	src_addr[INET_ADDRSTRLEN] = '\0';
	printf("Recived ICMP packet from %s\n", src_addr);

	hexdump(packet, len);

	puts("");
}

int
extract_src_addr(char *addrbuf, uint8_t *packet)
{
	packet += SRC_ADDR_OFFSET;

	if (inet_ntop(AF_INET, packet, addrbuf, INET_ADDRSTRLEN) == NULL)
		return -1;

	return 0;
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
	va_list		ap;

	va_start(ap, fmt);

	while (1) {
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
	}
}

void 
hexdump(uint8_t *buf, size_t len)
{
	size_t		linelen;

	if (buf == NULL)
		return;

	for (size_t i = 0; i < len; i += 16) {
		/* if remaining bytes is less than 8, use that instead */
		linelen = ((len - i) < 16) ? (len - i) : 16;		
		hexdump_line(buf, linelen);
		buf += linelen;
	}
}

void 
hexdump_line(uint8_t *line, size_t linelen)
{
	uint8_t		padding = 0;

	padding = HEXDUMP_LINELEN - linelen;

	for (int i = 0; i < linelen; i++)
		printf("%.2x ", line[i]);

	for (int i = 0; i < padding; i++)
		printf("   ");
	putchar('\t');

	putchar('|');
	for (int i = 0; i < linelen; i++) {
		if (isprint(line[i]))
			putchar(line[i]);
		else
			putchar('.');
	}

	printf("|\n");
}
