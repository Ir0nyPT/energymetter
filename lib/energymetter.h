
#include "hardware/adc.h"
#include <iostream>
#define pin_to_adc_channel(pin) (pin - 26)

#define ganho_fase 0.12695

class energymetter
{
  public:
    energymetter(/* args */);

    void Update();

    float GetVoltage_RMS() { return tensao_rms_; }
    float GetCurrent();
    float GetPowerFactor();
    uint8_t GetFrequency() { return 1 / periodo; }

    float potencia_inst();

    uint32_t potencia_total() { return tensao_rms_ * corrente_ * fp_; }
    void reset_total();

  private:
    float tensao_rms_{0.0f};
    float corrente_{0.0f};
    float fp_{1.0f};
    uint16_t periodo{0};

    int16_t tensao_max{0};
    int16_t tensao_anterior{0};
    uint16_t cnt = 0;
};

// float ler_tensao(uint16_t volt_read)
// {

//     if (volt_read > volt_max)
//     {
//         volt_max = volt_read;
//     }

//     if (volt_read < volt_min)
//     {
//         volt_min = volt_read;
//     }

//     if (volt_read > 2048 && volt_anterior < 2048)
//     {
//         // tensao = 266.3317f * volt_max/(1<<12); //melhor para humano entender

//         volt_max = volt_read;
//         volt_min = volt_read;
//     }

//     volt_anterior = volt_read;
// }

// uint16_t volt_max = 0;
// uint16_t volt_min = 4096;
// uint16_t volt_anterior = 0;

// /******TENSÃO***************/

// adc_select_input(0);
// ler_tensao(adc_read());

// uint16_t volt_read = adc_read();  // leitura

// if (volt_read > volt_max)
// {
//     volt_max = volt_read;
// }

// if (volt_read < volt_min)
// {
//     volt_min = volt_read;
// }

// if (volt_read > 2048 && volt_anterior < 2048)
// {
//     // tensao = 266.3317f * volt_max/(1<<12); //melhor para humano entender

//     volt_max = volt_read;
//     volt_min = volt_read;
// }

// volt_anterior = volt_read;