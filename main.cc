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
#include <stdio.h>
#include "hardware/adc.h"
#include <unistd.h> //Library for sleep() function
#include "hardware/timer.h"//library for timer
#include "hardware/adc.h"

using namespace std;



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

            while(!multicore_fifo_rvalid());

            uint32_t tensao = multicore_fifo_pop_blocking();
              
            std::cout << "Tensão: " << tensao << std::endl; 
        }

        else if (strcmp(buffer, "t") == 0)
        {
            multicore_fifo_push_blocking('t');

            while(!multicore_fifo_rvalid());

            uint32_t temperatura = multicore_fifo_pop_blocking();
              
            std::cout << "Temperatura: " << temperatura << std::endl; 
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
    
    uint16_t volt_max = 0;
    uint16_t volt_min = 4096;
    uint16_t volt_anterior = 0;

   const uint8_t led_pin = 18;
    
   gpio_init(led_pin);
    
   gpio_set_dir(led_pin, 1);

   //bool led_state = false; //flag inicializa

    /*INITS ADC*/

    adc_init();
    adc_gpio_init (30); //INICIALIZAÇAO COMO ANALOGICO TEMP
    adc_gpio_init (26); //INIT L 0U N
    adc_gpio_init (27);
    adc_set_temp_sensor_enabled(1);//ENABLE TEMPERATURE
    
    /******VARIAVEIS*******/

    //float tensao = 0;
    
    
    while (1)
    {
     

        // Print
        //std::cout<<"HELLO"<< std::endl;
        //busy_wait_ms(1000);
        
        //Toogle Led

        //gpio_put(led_pin, led_state); //so aqui altera o estado
        //led_state =! led_state; //guarda so o estado
    
    
        //Delay
        //busy_wait_ms(1000);

      

        //const float conversion_factor = 3.3f / (1 << 12);
        //uint16_t result = adc_read();
        adc_select_input (4);//SELECÇÃO DO ADC A LER;
        float temp_voltage = 3.3f * adc_read()/(1<<12);
        
        float temperatura = 27 - (temp_voltage - 0.706)/0.001721;
        
       // printf("A temp do ar é :   %f \n", temperature);
        

        /******TENSÃO***************/

        adc_select_input (0);

        uint16_t volt_read = adc_read(); //leitura 

        if (volt_read>volt_max)
        {
            volt_max = volt_read;
        }

        if(volt_read<volt_min)
        {
            volt_min = volt_read;
        }
        
        

        if(volt_read>2048 && volt_anterior<2048)
        {
           //tensao = 266.3317f * volt_max/(1<<12); //melhor para humano entender

           volt_max = volt_read;
           volt_min = volt_read;
        }

        volt_anterior = volt_read;

        if(multicore_fifo_rvalid())
        {
            uint32_t cmd = multicore_fifo_pop_blocking();

            switch (cmd)
            {
            case 't':
               multicore_fifo_push_blocking(static_cast<uint32_t>(temperatura*10.0f));
                break;

            case 'v':
               multicore_fifo_push_blocking(static_cast<uint32_t>(volt_read));
                break;
            
            default:
                break;
            }
        
        }
        





    }
    
       
return 0;
}









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


