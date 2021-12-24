#pragma once

#include <array>
#include <memory>

#include "ledarray.hpp"


class LEDImage {
public:
    LEDImage();

    void Clear();
private:
    typedef std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> Channel;

    std::unique_ptr<Channel> red;
    std::unique_ptr<Channel> green;
    std::unique_ptr<Channel> blue;
};