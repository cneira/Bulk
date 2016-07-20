#pragma once
#include <cstddef>

extern "C" {
#include <e_dma.h>
}

extern unsigned dma_data_size[8]; // in epiphany library

namespace bulk {
namespace epiphany {

class dma_task {
  public:
    dma_task() : config(0) {}

    dma_task(void* dst, const void* src, size_t nbytes) {
        set(dst, src, nbytes);
    }

    ~dma_task() { wait(); }

    // No copy constructor as the interrupt would only
    // signal one of the two tasks to non-busy
    dma_task(dma_task& other) = delete;
    void operator=(dma_task& other) = delete;

    // No move constructor because the interrupt will have
    // a pointer to the OLD object
    dma_task(dma_task&& other) = delete;
    void operator=(dma_task&& other) = delete;

    // Can only be called when the task is NOT busy
    // engine MUST be 0 or 1, this is not checked
    void push(void* dst, const void* src, size_t nbytes, int engine = 0) {
        set(dst, src, nbytes);
        push(engine);
    }

    // Wait for this task to complete
    void wait() const {
        // The interrupt handler will disable this specific bit
        volatile unsigned* conf = (volatile unsigned*)&config;
        while (*conf & E_DMA_ENABLE) {
        }
    }

    // This can only be called when the task NOT busy
    void set(void* dst, const void* src, size_t nbytes) {
        // Alignment
        unsigned index =
            (((unsigned)dst) | ((unsigned)src) | ((unsigned)nbytes)) & 7;
        unsigned shift = dma_data_size[index] >> 5;

        config =
            E_DMA_MASTER | E_DMA_ENABLE | E_DMA_IRQEN | dma_data_size[index];
        if ((((unsigned)dst) & 0xfff00000) == 0)
            config |= E_DMA_MSGMODE;
        inner_stride = 0x00010001 << shift;
        count = 0x00010000 | (nbytes >> shift);
        outer_stride = 0;
        src_addr = (void*)src;
        dst_addr = (void*)dst;
    }

    // Push this task to the dma engine
    // Assumes the task has been set through `set`
    // engine MUST be 0 or 1, this is not checked
    void push(int engine);

  private:
    // This must be unchanged, its how the hardware reads it
    // Nothing can be added or removed
    unsigned config;
    unsigned inner_stride;
    unsigned count;
    unsigned outer_stride;
    void* src_addr;
    void* dst_addr;

    friend void _dma_interrupt_(int);
} __attribute__((aligned(8)));

} // namespace epiphany
} // namespace bulk
