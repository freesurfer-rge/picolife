#pragma once

#include <array>
#include <memory>

#include "ledarray.hpp"

/*
 Note that the colour channels are heap allocated so these
 objects should be safe to stack allocate
*/

class LEDImage
{
public:
    LEDImage();

    void Clear();

    void SetPixel(const unsigned int ix, const unsigned int iy,
                  const uint8_t r, const uint8_t g, const uint8_t b);

    void SendToLEDArray(LEDArray &target) const;

private:
    typedef std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> Channel;

    std::unique_ptr<Channel> red;
    std::unique_ptr<Channel> green;
    std::unique_ptr<Channel> blue;
};