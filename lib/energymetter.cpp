#include "energymetter.h"
#include "../pin_defines.h"

energymetter::energymetter(/* args */)
{
    /*INITS ADC*/
    adc_init();
    adc_gpio_init(pin_adc_Fase);    // Inicializa Pin Leitura Fase
    adc_gpio_init(pin_adc_Neutro);  // Inicializa Pin Leitura Neutro
}

void energymetter::Update()
{
    /* Medir Tensão */

    // 1º Ler Analógico
    adc_select_input(pin_to_adc_channel(pin_adc_Fase));
    int16_t tensao = adc_read() - 2048;
    adc_select_input(pin_to_adc_channel(pin_adc_Neutro));
    tensao += adc_read() - 2048;

    cnt++;

    // 2º Verificar se iniciou um novo periodo
    if (tensao <= 0 && tensao_anterior > 0)
    {
        // Periodo do ciclo que decorreu
        periodo_ms = cnt * 2 / 10;
        cnt = 0;

        // Tensão máximo do ciclo qie decorreu
        if (tensao_max > 750)
        {
            tensao_rms_ = static_cast<float>(tensao_max) * ganho_fase;
        }
        else
        {
            tensao_rms_ = 0.0f;
        }
        tensao_max = 0;
    }
    tensao_anterior = tensao;

    // 3º Reter o pico máximo
    if (tensao > tensao_max)
    {
        tensao_max = tensao;
    }

    /* Medir Corrente */
}