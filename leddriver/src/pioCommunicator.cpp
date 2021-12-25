//#include <iostream>
//#include <bitset>

#include "leddriver/pioCommunicator.hpp"

namespace LEDDriver

{
    void PIOCommunicator::write32blocking(const uint32_t *src, const size_t n)
    {
        size_t tx_remaining = n;

        auto txfifo = reinterpret_cast<io_rw_32 *>(&(this->pio->txf[this->sm]));

        while (tx_remaining)
        {
            if (tx_remaining && !pio_sm_is_tx_fifo_full(this->pio, this->sm))
            {
                const uint32_t nxtWrite = *src++; // Note that src is a pointer to const, but not a const pointer
                // std::bitset<32> writeBits(nxtWrite);
                // std::cout << "Writing " << writeBits << std::endl;
                *txfifo = nxtWrite;
                --tx_remaining;
            }
        }
    }

    void PIOCommunicator::waitTXdrain()
    {
        do
        {
            ; /* Spinwait */
        } while (!pio_sm_is_tx_fifo_empty(this->pio, this->sm));
    }
}