#include "ledimage.hpp"

LEDImage::LEDImage() : 
red(std::make_unique<Channel>()),
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