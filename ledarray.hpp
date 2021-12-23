#pragma once

#include <array>

#include "hardware/pio.h"

#include "pioCommunicator.hpp"

class LEDArray
{
public:
    static constexpr float pio_freq = 50 * 1024 * 1024; // Hz

    static const unsigned int nRows = 32;
    static const unsigned int nCols = 32;
    static const unsigned int nPixelsPerWord = 4;
    static const unsigned int nWordsPerRow = nCols / nPixelsPerWord;
    static const unsigned int nFrames = 16;

    LEDArray(PIO targetPio);

    void UpdateBuffer(
        const std::array<uint8_t, nCols * nRows> &red,
        const std::array<uint8_t, nCols * nRows> &green,
        const std::array<uint8_t, nCols * nRows> &blue);

    void SendBuffer();

private:
    PIOCommunicator comms;

    std::array<uint32_t, (nRows / 2) * (nWordsPerRow * nFrames)> output_buffer;

    bool is_pixel_on(const uint8_t value, const unsigned int iFrame) const;
    std::array<uint32_t, LEDArray::nWordsPerRow> ConstructRowPair(
        const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &red,
        const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &green,
        const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &blue,
        const unsigned int iFrame,
        const unsigned int iRow) const;
};