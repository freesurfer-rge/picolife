#pragma once

class ColourVector
{
public:
    ColourVector(const float kx, const float ky, const float omega,
                 const float A, const float offset);

    unsigned char GetColour(const unsigned int ix, const unsigned int iy, const unsigned int itCount) const;

private:
    float kx, ky, omega;
    float A, offset;
};