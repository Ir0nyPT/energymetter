
#include "lwip/dns.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include <string.h>
#include <time.h>

#define NTP_SERVER "pool.ntp.org"
#define NTP_MSG_LEN 48
#define NTP_PORT 123
#define NTP_DELTA 2208988800  // seconds between 1 Jan 1900 and 1 Jan 1970

class NTP_Time
{
  public:
    NTP_Time(const char* ntp_server = NTP_SERVER) : ntp_server_{ntp_server} {}
    NTP_Time(const char* ssid, const char* pw, const char* ntp_server = NTP_SERVER) : ntp_server_{ntp_server}
    {
        cyw43_arch_init();
        cyw43_arch_enable_sta_mode();
        cyw43_arch_wifi_connect_timeout_ms(ssid, pw, CYW43_AUTH_WPA2_AES_PSK, 10000);
    }

    tm Sync_NTP()
    {
        NTP_T* state = (NTP_T*)calloc(1, sizeof(NTP_T));

        state->ntp_pcb = udp_new_ip_type(IPADDR_TYPE_V4);
        udp_recv(state->ntp_pcb, ntp_recv, state);

        cyw43_arch_lwip_begin();
        dns_gethostbyname(ntp_server_, &state->ntp_server_address, ntp_dns_found, state);
        cyw43_arch_lwip_end();

        ntp_request(state);

        state->dns_request_sent = true;

        uint8_t tries{20};
        while (state->dns_request_sent && tries--)
        {
            cyw43_arch_poll();
            cyw43_arch_wait_for_work_until(at_the_end_of_time);
            sleep_ms(100);
        }

        free(state);

        return state->now;
    }

    ~NTP_Time()
    {
        if (flag_wifi_to_disconnect)
            cyw43_arch_deinit();
    }

  private:
    bool flag_wifi_to_disconnect{false};
    const char* ntp_server_{NTP_SERVER};

    typedef struct NTP_T_
    {
        ip_addr_t ntp_server_address;
        bool dns_request_sent;
        struct udp_pcb* ntp_pcb;
        tm now;
    } NTP_T;

    // Called with results of operation
    static void ntp_result(NTP_T* state, int status, time_t* result)
    {
        if (status == 0 && result)
        {
            struct tm* utc = gmtime(result);
            state->now.tm_mday = utc->tm_mday;
            state->now.tm_mon = utc->tm_mon + 1;
            state->now.tm_year = utc->tm_year + 1900;
            state->now.tm_hour = utc->tm_hour;
            state->now.tm_min = utc->tm_min;
            state->now.tm_sec = utc->tm_sec;
        }

        state->dns_request_sent = false;
    }

    // Make an NTP request
    static void ntp_request(NTP_T* state)
    {
        cyw43_arch_lwip_begin();
        struct pbuf* p = pbuf_alloc(PBUF_TRANSPORT, NTP_MSG_LEN, PBUF_RAM);
        uint8_t* req = (uint8_t*)p->payload;
        memset(req, 0, NTP_MSG_LEN);
        req[0] = 0x1b;
        udp_sendto(state->ntp_pcb, p, &state->ntp_server_address, NTP_PORT);
        pbuf_free(p);
        cyw43_arch_lwip_end();
    }

    // Call back with a DNS result
    static void ntp_dns_found(const char* hostname, const ip_addr_t* ipaddr, void* arg)
    {
        NTP_T* state = (NTP_T*)arg;
        if (ipaddr)
        {
            state->ntp_server_address = *ipaddr;
            ntp_request(state);
        }
    }

    // NTP data received
    static void ntp_recv(void* arg, struct udp_pcb* pcb, struct pbuf* p, const ip_addr_t* addr, u16_t port)
    {
        NTP_T* state = (NTP_T*)arg;
        uint8_t mode = pbuf_get_at(p, 0) & 0x7;
        uint8_t stratum = pbuf_get_at(p, 1);

        // Check the result
        if (ip_addr_cmp(addr, &state->ntp_server_address) && port == NTP_PORT && p->tot_len == NTP_MSG_LEN &&
            mode == 0x4 && stratum != 0)
        {
            uint8_t seconds_buf[4] = {0};
            pbuf_copy_partial(p, seconds_buf, sizeof(seconds_buf), 40);
            uint32_t seconds_since_1900 =
                seconds_buf[0] << 24 | seconds_buf[1] << 16 | seconds_buf[2] << 8 | seconds_buf[3];
            uint32_t seconds_since_1970 = seconds_since_1900 - NTP_DELTA;
            time_t epoch = seconds_since_1970;
            ntp_result(state, 0, &epoch);
        }
        pbuf_free(p);
    }
};