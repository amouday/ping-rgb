idf_component_register(
	SRCS "net-rgb.c"
    INCLUDE_DIRS "."
	PRIV_REQUIRES esp_wifi
	PRIV_REQUIRES esp_event
	PRIV_REQUIRES esp_netif
	PRIV_REQUIRES nvs_flash
)
