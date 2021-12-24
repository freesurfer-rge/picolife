#include <cmath>

#include "colourvector.hpp"

ColourVector::ColourVector(const float kx,
                           const float ky,
                           const float omega,
                           const float A,
                           const float offset) : kx(kx),
                                                 ky(ky),
                                                 omega(omega),
                                                 A(A),
                                                 offset(offset)
{
    // Nothing to do
}

unsigned char
ColourVector::GetColour(const unsigned int ix, const unsigned int iy, const unsigned int itCount) const
{
    float positionPhase = (ix * this->kx) + (iy + this->ky);
    float timePhase = omega * itCount;

    float raw = sinf(positionPhase - timePhase);

    return offset + A * raw;
}