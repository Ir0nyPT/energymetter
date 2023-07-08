#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"  //library for timer
#include "pico.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pin_defines.h"
#include <stdio.h>

#include "lib/core1.h"
#include "lib/energymetter.h"

#define timer_sync_us 200
#define timer_sync_us_adjust -3

volatile bool flag_timer{false};

static bool timer_IRQ(struct repeating_timer* t)
{
    /* Flag Timer */
    flag_timer = true;

    return true;
}

void init()
{
    /* Initialise I/O */
    stdio_init_all();

    /* Status PIN */
    gpio_init(pin_led_1Hz);
    gpio_set_dir(pin_led_1Hz, 1);

    /* Timer Interrupt */
    static struct repeating_timer timerClock;
    add_repeating_timer_us(timer_sync_us + timer_sync_us_adjust, timer_IRQ, NULL, &timerClock);

    /* Initialise Second core with Serial in Scan*/
    multicore_launch_core1(serialInput);

    /* Inicializar módulo WIFI */
    // cyw43_arch_init();
}

int main()
{
    init();

    energymetter contador_energia;

    bool led_state = false;
    uint16_t cnt = 0;

    while (1)
    {
        /* Trigger pelo timer (200 us) */
        if (flag_timer)
        {
            /* Analisa Energia */
            contador_energia.Update();

            /* Led 1 Hz (500 ms) */
            if (cnt >= 2500)
            {
                gpio_put(pin_led_1Hz, led_state);  // so aqui altera o estado
                led_state = !led_state;            // guarda so o estado
                cnt = 0;
            }
            else
            {
                cnt++;
            }

            flag_timer = false;
        }

        /* Serial */
        if (multicore_fifo_rvalid())
        {
            uint32_t cmd = multicore_fifo_pop_blocking();

            switch (cmd)
            {
                case 'v':
                    multicore_fifo_push_blocking(static_cast<uint32_t>(contador_energia.GetVoltage_RMS() * 10.0f));
                    break;
                case 'f':
                    multicore_fifo_push_blocking(static_cast<uint32_t>(contador_energia.GetFrequency()));
                    break;

                default:
                    break;
            }
        }
    }

    return 0;
}

/* TEMPERATURA

INIT:
    adc_gpio_init(30);  // INICIALIZAÇAO COMO ANALOGICO TEMP
    adc_set_temp_sensor_enabled(1);  // ENABLE TEMPERATURE

Run:
    adc_select_input(4);  // SELECÇÃO DO ADC A LER;
    float temp_voltage = 3.3f * adc_read() / (1 << 12);

    float temperatura = 27 - (temp_voltage - 0.706) / 0.001721;
*/

/*
DESAFIO 1:

- c, c++ mostrar fazer programas
- algoritmo recebe corrente e tensao e de-me potencia ao longo
- pensar aplicação resistencias em parelo, serie,
- manual da aplicação - fluxograma


DESAFIO 2

- programa tipo ler entrada do microcontrolador
- potenciometro no micro; (LER DOCS, CODIGO EXEMPLO)
- Timers;
- adc;
- interrupt;


DESAFIO 3

- perceber o prjecto/ fluxograma/ passos a usar .....
- pensar como se faze muito burro;
- achar v max , por tempo;
- achar i max, por tempo;
- calcular potencia....
- integrar como fazer no micro....
(SOMATORIO AO LONGO DO TEMPO);

- DESAFIO
- DUVIDAS
- DEBATE

*/
