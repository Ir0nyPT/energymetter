
#include "hardware/gpio.h"
#include "pico.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

void serialInput()
{
    printf("Second Core Running !\n");
    char buffer[16];

    while (1)
    {
        scanf("%16s", buffer);

        if (strcmp(buffer, "reboot") == 0)
        {
            std::cout << "Going to Boot Sel Mode" << std::endl;
            // Change Raspberry pico to bootsel mode
            reset_usb_boot(0, 0);
        }
        else
        {
            std::cout << "Received: " << buffer << std::endl;
        }
    }
}

int main()
{

    /* Initialise I/O */
    stdio_init_all();

    /* Initialise Second core with Serial in Scan*/
    multicore_launch_core1(serialInput);

    const uint8_t led_pin = 25;

    gpio_init(led_pin);
    gpio_set_dir(led_pin, 1);

    bool led_state = false;

    uint32_t cnt = 0;
    while (1)
    {
        // Print
        std::cout << "Ola Marco " << cnt++ << std::endl;

        // Toogle Led
        gpio_put(led_pin, led_state);
        led_state = !led_state;

        // Delay
        busy_wait_ms(1000);
    }
    return 0;
}