#include <iostream>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "leddriver/ledimage.hpp"

uint8_t value_for_row(const unsigned int iRow)
{
    return iRow / 2;
}

LEDDriver::LEDImage CreateTestCard()
{
    LEDDriver::LEDImage result;

    for (unsigned int iSquare = 0; iSquare < 64; iSquare++)
    {
        unsigned int sx = iSquare % 8;
        unsigned int sy = iSquare / 8;

        for (unsigned int ix = 0; ix < 4; ++ix)
        {
            for (unsigned int iy = 0; iy < 4; ++iy)
            {
                unsigned int idxX = sx * 4 + ix;
                unsigned int idxY = sy * 4 + iy;

                uint8_t r = (iSquare & 1) ? value_for_row(idxY) : 0;
                uint8_t g = (iSquare & 2) ? value_for_row(idxY) : 0;
                uint8_t b = (iSquare & 4) ? value_for_row(idxY) : 0;
                result.SetPixel(idxX, idxY, r, g, b);
            }
        }
    }

    return result;
}

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    std::cout << "LED Driver Testcard" << std::endl;

    LEDDriver::LEDArray *ledArr = new LEDDriver::LEDArray(pio0);

    auto img = CreateTestCard();
    img.SendToLEDArray(*ledArr);
    while (true)
    {
        auto targetTime = make_timeout_time_ms(5);
        ledArr->SendBuffer();
        sleep_until(targetTime);
    }
}