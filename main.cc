/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/bootrom.h"

#include "NTC_Time.h"
#include "lib/mqtt_client.h"

int main()
{
    stdio_init_all();

    auto utc = NTP_Time("NoNamed", "Drix&Deri15", "192.168.50.1").Sync_NTP();

    printf("Now: %02d/%02d/%04d %02d:%02d:%02d\n",
           utc.tm_mday,
           utc.tm_mon,
           utc.tm_year,
           utc.tm_hour,
           utc.tm_min,
           utc.tm_sec);

    reset_usb_boot(0, 0);
    return 0;
}