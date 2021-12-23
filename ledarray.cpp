#include <algorithm>
#include <bitset>
#include <set>

// Headers from the Pico SDK
#include "hardware/clocks.h"
#include "pico/stdlib.h"

// Autogenerated header from CMake
#include "led_line.pio.h"

#include "ledarray.hpp"

const uint firstColourPin = 2;
enum ColourPinOffsets
{
    blue2 = 0,
    red2 = 1,
    blue1 = 2,
    red1 = 3,
    green1 = 4,
    green2 = 5
};

enum RowPins
{
    rowA = 9,
    rowB = 10,
    rowC = 11,
    rowD = 12
};

enum ControlPins
{
    clk = 0,
    latch = 1,
    outputEnable = 8
};

LEDArray::LEDArray(PIO targetPio) : comms(),
                                    output_buffer()
{
    std::set<uint> outputPins = {
        clk, latch, outputEnable,
        red1 + firstColourPin,
        red2 + firstColourPin,
        green1 + firstColourPin,
        green2 + firstColourPin,
        blue1 + firstColourPin,
        blue2 + firstColourPin,
        rowA, rowB, rowC, rowD};

    // Configure pins as outputs and set low
    for (auto p : outputPins)
    {
        gpio_init(p);
        gpio_set_dir(p, GPIO_OUT);
        gpio_put(p, 0);
    }
    // Bit bang something
    gpio_put(blue1, true);
    gpio_put(clk, true);
    gpio_put(latch, true);
    sleep_ms(1000);
    for (auto p : outputPins)
    {
        gpio_put(p, 0);
    }

    // Configure the PIO
    uint sm = pio_claim_unused_sm(targetPio, true);
    uint offset = pio_add_program(targetPio, &led_line_program);

    float divider = (float)clock_get_hz(clk_sys) / pio_freq;

    led_line_init(targetPio, sm, offset, divider, ControlPins::clk, firstColourPin);

    pio_sm_set_enabled(targetPio, sm, true);

    this->comms.pio = targetPio;
    this->comms.sm = sm;
    this->comms.pin_clk = ControlPins::clk;
    this->comms.pin_data = firstColourPin;
}

void LEDArray::UpdateBuffer(
    const std::array<uint8_t, nCols * nRows> &red,
    const std::array<uint8_t, nCols * nRows> &green,
    const std::array<uint8_t, nCols * nRows> &blue)
{
    // We send rows out two at a time, separated by 16 rows
    for (unsigned int iRow = 0; iRow < (nRows / 2); ++iRow)
    {
        std::array<uint32_t, nWordsPerRow * nFrames> tmp_buffer;

        for (unsigned int iFrame = 0; iFrame < nFrames; ++iFrame)
        {
            auto nextRow = this->ConstructRowPair(red, green, blue, iFrame, iRow);
            std::copy(nextRow.begin(), nextRow.end(), tmp_buffer.begin() + (iFrame * nWordsPerRow));
        }

        std::copy(tmp_buffer.begin(), tmp_buffer.end(), this->output_buffer.begin() + (iRow * (nWordsPerRow * nFrames)));
    }
}

std::array<uint32_t, LEDArray::nWordsPerRow> LEDArray::ConstructRowPair(
    const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &red,
    const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &green,
    const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &blue,
    const unsigned int iFrame,
    const unsigned int iRow) const
{
    /*
    Constructs an output buffer for a pair of rows.

    Each output word has 24 active bits: 2 rows * 4 pixels * 3 colours
    */
    std::array<uint32_t, LEDArray::nWordsPerRow> result;

    for (unsigned int i = 0; i < LEDArray::nWordsPerRow; ++i)
    {
        std::bitset<32> nxtWord(0);

        for (unsigned int j = 0; j < LEDArray::nPixelsPerWord; ++j)
        {
            const unsigned int idxX = j + (LEDArray::nPixelsPerWord * i);

            unsigned int linearIdx = (iRow * LEDArray::nCols) + idxX;
            nxtWord[(j * 6) + ColourPinOffsets::red1] = this->is_pixel_on(red.at(linearIdx), iFrame);
            nxtWord[(j * 6) + ColourPinOffsets::green1] = this->is_pixel_on(green.at(linearIdx), iFrame);
            nxtWord[(j * 6) + ColourPinOffsets::blue1] = this->is_pixel_on(blue.at(linearIdx), iFrame);

            linearIdx = ((iRow + (LEDArray::nRows / 2)) * LEDArray::nCols) + idxX;
            nxtWord[(j * 6) + ColourPinOffsets::red2] = this->is_pixel_on(red.at(linearIdx), iFrame);
            nxtWord[(j * 6) + ColourPinOffsets::green2] = this->is_pixel_on(green.at(linearIdx), iFrame);
            nxtWord[(j * 6) + ColourPinOffsets::blue2] = this->is_pixel_on(blue.at(linearIdx), iFrame);
        }
        result.at(i) = nxtWord.to_ulong();
    }

    return result;
}

bool LEDArray::is_pixel_on(const uint8_t value, const unsigned int iFrame) const
{
    return value > iFrame;
}

void LEDArray::SendBuffer()
{
    for (unsigned int i = 0; i < nRows / 2; i++)
    {
        gpio_put(ControlPins::outputEnable, true);
        gpio_put(RowPins::rowC, i & 8);
        gpio_put(RowPins::rowD, i & 4);
        gpio_put(RowPins::rowA, i & 2);
        gpio_put(RowPins::rowB, i & 1);
        for (unsigned int iFrame = 0; iFrame < nFrames; ++iFrame)
        {
            unsigned int idx = (iFrame * nWordsPerRow) + (i * nWordsPerRow * nFrames);
            comms.write32blocking(&(output_buffer.at(idx)), nWordsPerRow);
            comms.waitTXdrain();
            gpio_put(ControlPins::outputEnable, false);
            sleep_us(1);
        }
    }
}