#include "dns_resolver.h"
#include "lwip/apps/mqtt.h"
#include "lwip/dns.h"
#include "pico/cyw43_arch.h"

typedef struct mqtt_t
{
    mqtt_client_t* mqtt_client;    /* lwIP MQTT client handle */
    DnsResolver_info_t addr;       /* broker lwip address, resolved by DNS if hostname is used */
    unsigned char broker[32];      /* broker IP or hostname string. For hostname, DNS will be used */
    unsigned char client_id[32];   /* client ID used for connection */
    unsigned char client_user[32]; /* client user name used for connection */
    unsigned char client_pass[96]; /* client user password */
} mqtt_t;

static mqtt_t mqtt; /* information used for MQTT connection */

void MqttClient_Connect(void)
{
    mqtt.mqtt_client = mqtt_client_new(); /* create client handle */
}
