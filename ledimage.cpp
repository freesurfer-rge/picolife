#include "ledimage.hpp"

LEDImage::LEDImage() : red(std::make_unique<Channel>()),
                       green(std::make_unique<Channel>()),
                       blue(std::make_unique<Channel>())
{
    this->Clear();
}

void LEDImage::Clear()
{
    this->red->fill(0);
    this->green->fill(0);
    this->blue->fill(0);
}

void LEDImage::SetPixel(const unsigned int ix, const unsigned int iy,
                        const uint8_t r, const uint8_t g, const uint8_t b)
{
    const size_t idx = ix + (LEDArray::nCols * iy);
    this->red->at(idx) = r;
    this->green->at(idx) = g;
    this->blue->at(idx) = b;
}

void LEDImage::SendToLEDArray(LEDArray &target) const
{
    target.UpdateBuffer(*(this->red), *(this->green), *(this->blue));
}