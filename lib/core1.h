#include "pico/bootrom.h"
#include "pico/multicore.h"
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

        else if (strcmp(buffer, "v") == 0)
        {
            multicore_fifo_push_blocking('v');

            while (!multicore_fifo_rvalid())
                ;

            uint32_t tensao = multicore_fifo_pop_blocking();

            std::cout << "Tensão: " << tensao << " V" << std::endl;
        }
        else if (strcmp(buffer, "f") == 0)
        {
            multicore_fifo_push_blocking('f');

            while (!multicore_fifo_rvalid())
                ;

            uint32_t freq = multicore_fifo_pop_blocking();

            std::cout << "Frequencia: " << freq << " Hz" << std::endl;
        }

        else
        {
            std::cout << "Received: " << buffer << std::endl;
        }
    }
}