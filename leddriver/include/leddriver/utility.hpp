#pragma once

#include "leddriver/ledimage.hpp"

namespace LEDDriver
{
    void LaunchOnCore1(LEDDriver::LEDArray *ledArr, const uint32_t refreshms);
}