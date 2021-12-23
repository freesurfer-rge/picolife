#include <iostream>
#include <set>
#include <bitset>
#include <array>
#include <algorithm>
#include <cmath>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "led_line.pio.h"

#include "pioCommunicator.hpp"

const uint clk = 0;
const uint latch = 1;

const uint red1 = 5;
const uint red2 = 3;
const uint blue1 = 4;
const uint blue2 = 2;
const uint green1 = 6;
const uint green2 = 7;
const uint firstDataPin = blue2;

const uint outputEnable = 8;

const uint rowA = 9;
const uint rowB = 10;
const uint rowC = 11;
const uint rowD = 12;

std::set<uint> outputPins = {
    clk, latch, outputEnable,
    red1, red2, green1, green2, blue1, blue2,
    rowA, rowB, rowC, rowD};

const unsigned int nRows = 32;
const unsigned int nCols = 32;
const unsigned int nPixelsPerWord = 4;
const unsigned int nWordsPerRow = nCols / nPixelsPerWord;
const unsigned int nFrames = 32;

std::array<uint32_t, (nRows / 2) * (nWordsPerRow * nFrames)> output_buffer; // Each uint32 holds 4 pixels

const float gamma_correction = 1.2;

bool is_pixel_on(const uint8_t value, const unsigned int iFrame, const unsigned int nFrames)
{
    if (value == 0)
    {
        return false;
    }

    float threshold = (1.0f+iFrame) / nFrames;

    float scaledValue = powf(value / 255.0f, gamma_correction);

    return (scaledValue >= threshold);
}

void write_image_to_output_buffer(
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
            for (unsigned int i = 0; i < nWordsPerRow; ++i)
            {
                std::bitset<32> nxtWord(0);

                for (unsigned int j = 0; j < nPixelsPerWord; ++j)
                {
                    const unsigned int idxX = j + (nPixelsPerWord * i);

                    unsigned int linearIdx = (iRow * nCols) + idxX;
                    nxtWord[(j * 6) + (red1 - firstDataPin)] = is_pixel_on(red.at(linearIdx), iFrame, nFrames);
                    nxtWord[(j * 6) + (green1 - firstDataPin)] = is_pixel_on(green.at(linearIdx), iFrame, nFrames);
                    nxtWord[(j * 6) + (blue1 - firstDataPin)] = is_pixel_on(blue.at(linearIdx), iFrame, nFrames);

                    linearIdx = ((iRow + (nRows / 2)) * nCols) + idxX;
                    nxtWord[(j * 6) + (red2 - firstDataPin)] = is_pixel_on(red.at(linearIdx), iFrame, nFrames);
                    nxtWord[(j * 6) + (green2 - firstDataPin)] = is_pixel_on(green.at(linearIdx), iFrame, nFrames);
                    nxtWord[(j * 6) + (blue2 - firstDataPin)] = is_pixel_on(blue.at(linearIdx), iFrame, nFrames);
                }
                tmp_buffer.at(i + (nWordsPerRow * iFrame)) = nxtWord.to_ulong();
            }
        }
        std::copy(tmp_buffer.begin(), tmp_buffer.end(), output_buffer.begin() + (iRow * (nWordsPerRow * nFrames)));
    }
}


uint8_t value_for_row(const unsigned int iRow)
{
    if( iRow==nRows)
    {
        return 255;
    }

    float v = (1.0f + iRow) / nRows;

    return floor(v*255);
}

int main()
{
    stdio_init_all();
    std::cout << "LED Driver" << std::endl;

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

    const float pio_freq = 50 * 1024 * 1024; // Hz

    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &led_line_program);

    float divider = (float)clock_get_hz(clk_sys) / pio_freq;

    led_line_init(pio, sm, offset, divider, clk, firstDataPin);

    pio_sm_set_enabled(pio, sm, true);

    PIOCommunicator comms;
    comms.pio = pio;
    comms.sm = sm;
    comms.pin_clk = clk;
    comms.pin_data = firstDataPin;

    // Set up an image
    std::array<uint8_t, 32 * 32> red;
    std::array<uint8_t, 32 * 32> green;
    std::array<uint8_t, 32 * 32> blue;
    red.fill(0);
    green.fill(0);
    blue.fill(0);

    
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

                const unsigned int linearIdx = (nCols * idxY) + idxX;
                red.at(linearIdx) = (iSquare & 1) ? value_for_row(idxY) : 0;
                green.at(linearIdx) = (iSquare & 2) ? value_for_row(idxY) : 0;
                blue.at(linearIdx) = (iSquare & 4) ? value_for_row(idxY) : 0;
            }
        }
    }
    
    /*
    for (unsigned int iy = 0; iy < nRows; ++iy)
    {
        for (unsigned int ix = 0; ix < nCols; ++ix)
        {
            red[ix + (nCols * iy)] = 255;
            green[ix + (nCols * iy)] = ((ix) <= iy) * 255;
        }
    }
    */

    write_image_to_output_buffer(red, green, blue);

    std::cout << "Starting PIO" << std::endl;

    unsigned long itCount = 0;
    while (true)
    {
        // std::cout << "PIO running " << itCount << std::endl;

        for (unsigned int i = 0; i < nRows / 2; i++)
        {
            gpio_put(outputEnable, true);
            gpio_put(rowC, i & 8);
            gpio_put(rowD, i & 4);
            gpio_put(rowA, i & 2);
            gpio_put(rowB, i & 1);
            for (unsigned int iFrame = 0; iFrame < nFrames; ++iFrame)
            {
                unsigned int idx = (iFrame * nWordsPerRow) + (i * nWordsPerRow * nFrames);
                comms.write32blocking(&(output_buffer.at(idx)), nWordsPerRow);
                comms.waitTXdrain();
                gpio_put(outputEnable, false);
                sleep_us(1);
            }
        }
        itCount++;
    }

    return 0;
}
