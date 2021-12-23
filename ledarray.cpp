#include <algorithm>
#include <bitset>

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
            for (unsigned int i = 0; i < nWordsPerRow; ++i)
            {
                std::bitset<32> nxtWord(0);

                for (unsigned int j = 0; j < nPixelsPerWord; ++j)
                {
                    const unsigned int idxX = j + (nPixelsPerWord * i);

                    unsigned int linearIdx = (iRow * nCols) + idxX;
                    nxtWord[(j * 6) + ColourPinOffsets::red1] = this->is_pixel_on(red.at(linearIdx), iFrame, nFrames);
                    nxtWord[(j * 6) + ColourPinOffsets::green1] = this->is_pixel_on(green.at(linearIdx), iFrame, nFrames);
                    nxtWord[(j * 6) + ColourPinOffsets::red1] = this->is_pixel_on(blue.at(linearIdx), iFrame, nFrames);

                    linearIdx = ((iRow + (nRows / 2)) * nCols) + idxX;
                    nxtWord[(j * 6) + ColourPinOffsets::red2] = this->is_pixel_on(red.at(linearIdx), iFrame, nFrames);
                    nxtWord[(j * 6) + ColourPinOffsets::green2] = this->is_pixel_on(green.at(linearIdx), iFrame, nFrames);
                    nxtWord[(j * 6) + ColourPinOffsets::blue2] = this->is_pixel_on(blue.at(linearIdx), iFrame, nFrames);
                }
                tmp_buffer.at(i + (nWordsPerRow * iFrame)) = nxtWord.to_ulong();
            }
        }
        std::copy(tmp_buffer.begin(), tmp_buffer.end(), this->output_buffer.begin() + (iRow * (nWordsPerRow * nFrames)));
    }
}