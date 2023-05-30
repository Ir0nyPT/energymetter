#include <iostream>
#include <cstdint>


class energymetter
{
  public:
    energymetter();
    ~energymetter();
    void Update();
    uint8_t Get_currenteAtual();

  private:
    uint8_t voltage_;
    uint8_t current_;
};
