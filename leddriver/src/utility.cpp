#include "pico/multicore.h"
#include "pico/time.h"

#include "leddriver/utility.hpp"

namespace LEDDriver
{
    void core1Entry()
    {
        uint32_t fifo_value = multicore_fifo_pop_blocking();
        LEDDriver::LEDArray *ledArr = reinterpret_cast<LEDDriver::LEDArray *>(fifo_value);

        const uint32_t refreshms = multicore_fifo_pop_blocking();

        while (true)
        {
            auto targetTime = make_timeout_time_ms(refreshms);
            ledArr->SendBuffer();
            sleep_until(targetTime);
        }
    }

    void LaunchOnCore1(LEDDriver::LEDArray *ledArr, const uint32_t refreshms)
    {
        multicore_launch_core1(core1Entry);
        multicore_fifo_push_blocking(reinterpret_cast<uint32_t>(ledArr));
        multicore_fifo_push_blocking(refreshms);
    }
}