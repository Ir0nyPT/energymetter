#include "energymetter.h"

energymetter::energymetter()
{
    std::cout << "Criacao da class Contador de Energia" << std::endl;
}

energymetter::~energymetter()
{
    std::cout << "Destruicao da class Contador de Energia" << std::endl;
}

void energymetter::Update()
{
    // Ler ADC .... etc ....
    voltage_++;
    current_ += 2;
}

uint8_t energymetter::Get_currenteAtual()
{
    // Codigo....
    return current_;
}