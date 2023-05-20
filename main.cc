
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "lib/energymetter.h"
#include "pico.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pin_defines.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

energymetter contador;

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
        else if (strcmp(buffer, "current") == 0)
        {
            printf("Current: %dA\n", contador.Get_currenteAtual());
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

    /* LED */
    gpio_init(pin_led_1Hz);
    gpio_set_dir(pin_led_1Hz, OUTPUT);

    gpio_put(pin_led_1Hz, 0);

    bool led_state = false;

    /* ANALOG */
    adc_init();
    adc_gpio_init(30);
    adc_select_input(4);
    adc_set_temp_sensor_enabled(1);

    while (1)
    {
        // Update Energy Values
        contador.Update();

        // Print
        int temp_adc = adc_read();
        float temp_volt = temp_adc * (3.3f / 4096.0f);
        int temp = 27 - (temp_volt - 0.706) / 0.001721;

        std::cout << "ADC: " << temp_adc << "   Voltagem: " << temp_volt << "V   Temperatura: " << temp << "ºC"
                  << std::endl;

        // Toogle Led
        gpio_put(pin_led_1Hz, led_state);
        led_state = !led_state;

        // Delay
        busy_wait_ms(1000);
    }
    return 0;
}
