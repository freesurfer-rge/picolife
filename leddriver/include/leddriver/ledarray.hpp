#pragma once

#include <array>
#include <atomic>
#include <memory>

#include "hardware/pio.h"

#include "leddriver/pioCommunicator.hpp"

namespace LEDDriver
{
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

        typedef std::array<uint32_t, (nRows / 2) * (nWordsPerRow * nFrames)> Buffer;

    private:
        std::atomic<unsigned char> activeBuffer;
        LEDDriver::PIOCommunicator comms;

        std::unique_ptr<Buffer> outputBuffer0, outputBuffer1;

        bool is_pixel_on(const uint8_t value, const unsigned int iFrame) const;
        std::array<uint32_t, LEDArray::nWordsPerRow> ConstructRowPair(
            const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &red,
            const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &green,
            const std::array<uint8_t, LEDArray::nCols * LEDArray::nRows> &blue,
            const unsigned int iFrame,
            const unsigned int iRow) const;

        void EnableDisplay() const;
        void DisableDisplay() const;
        void SelectRow(const unsigned int iRow) const;
    };
}