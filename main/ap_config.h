#ifndef		__AP_CONFIG_H
#define		__AP_CONFIG_H

#include "esp_wifi.h"

#define AP_SSID				"Spectrum Wifi32"
#define AP_PASSWORD			"spectrum32"
#define AP_SSID_LEN			0
#define AP_CHANNEL			0
#define AP_AUTH_MODE		WIFI_AUTH_WPA2_PSK
#define AP_MAX_CONNECTION	10

#define AP_IP_ADDRESS		"192.168.10.1"
#define AP_IP_NETMASK		"255.255.255.0"

#endif
