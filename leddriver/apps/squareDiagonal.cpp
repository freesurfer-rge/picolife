#include <iostream>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "leddriver/ledimage.hpp"

uint8_t value_for_row(const unsigned int iRow)
{
    return iRow / 2;
}

LEDDriver::LEDImage CreateSquareDiagonal()
{
    LEDDriver::LEDImage result;

    for (unsigned int iy = 0; iy < LEDDriver::LEDArray::nRows; ++iy)
    {
        for (unsigned int ix = 0; ix < LEDDriver::LEDArray::nCols; ++ix)
        {
            uint8_t r = LEDDriver::LEDArray::nFrames - value_for_row(iy);
            uint8_t g = ((LEDDriver::LEDArray::nCols - 1) - ix == iy) ? 255 : 0;
            uint8_t b = ((ix) <= iy) * value_for_row(iy);

            result.SetPixel(ix, iy, r, g, b);
        }
    }

    return result;
}

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    std::cout << "LED Driver SquareDiagonal" << std::endl;

    LEDDriver::LEDArray ledArr(pio0);

    auto img = CreateSquareDiagonal();
    img.SendToLEDArray(ledArr);
    while (true)
    {
        auto targetTime = make_timeout_time_ms(5);
        ledArr.SendBuffer();
        sleep_until(targetTime);
    }
}