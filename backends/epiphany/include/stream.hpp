#pragma once
#include "backend.hpp"
#include "dma.hpp"

namespace bulk {
namespace epiphany {

class stream {
  public:
    stream() { stream_id = -1; }
    stream(int id) {
        stream_id = -1;
        open(id);
    }
    ~stream() { close(); }

    // See `dma_task` for why it can not be copied or moved
    stream(stream&) = delete;
    void operator=(stream&) = delete;
    stream(stream&&) = delete;
    void operator=(stream&&) = delete;

    void open(int id);

    void close() {
        if (stream_id == -1)
            return;
        wait();
        // Should not have to lock mutex for this atomic write
        combuf_->streams[stream_id].pid = -1;
        stream_id = -1;
    }

    bool is_valid() const { return stream_id != -1; }

    operator bool() const { return is_valid(); }

    // Wait for any pending asynchroneous operations to complete
    void wait() { dma.wait(); }

    // Seek with relative offset
    void seek_rel(int delta_bytes) {
        if (delta_bytes <= 0) {
            cursor = (void*)((unsigned)cursor + delta_bytes);
            if (cursor < buffer) {
                // TODO: host request
                cursor = buffer;
            }
        } else {
            size_t remaining = (unsigned)buffer + size - (unsigned)cursor;
            if ((unsigned)delta_bytes > remaining) {
                // TODO: host request
                delta_bytes = remaining;
            }
            cursor = (void*)((unsigned)cursor + delta_bytes);
        }
    }

    // Seek with absolute offset from the start of the stream
    void seek_abs(size_t offset_) {
        // The buffer
        // [buffer, buffer + capacity[
        // corresponds to the stream part
        // [offset, offset + capacity[
        // Now we request to go to `offset_`
        if ((int)offset_ < offset || offset_ > offset + capacity) {
            // TODO: host seek request
            offset = capacity;
        }
        cursor = (void*)((unsigned)buffer + offset);
    }

    /**
     * Write local data up to a stream.
     *
     * @param data The data to be sent up the stream
     * @param data_size The size of the data to be sent, i.e. the size of the
     * token.
     * @param wait_for_completion If true this function blocks untill
     * the data is completely written to the stream.
     * @return Number of bytes written, negative on error.
     *
     * The function *always* waits for any previous transfers to finish.
     *
     * If `wait_for_completion` is nonzero, this function will wait untill
     * the data is transferred. This corresponds to single buffering.
     *
     * Alternativly, double buffering can be used as follows.
     * Set `wait_for_completion` to zero and continue constructing the next
     * token
     * in a different buffer. Usage example:
     * \code{.c}
     * int* buf1 = new int[100];
     * int* buf2 = new int[100];
     * int* curbuf = buf1;
     * int* otherbuf = buf2;
     *
     * stream s(0); // open strema 0
     * while (...) {
     *     // Fill curbuf
     *     for (int i = 0; i < 100; i++)
     *         curbuf[i] = 5;
     *
     *     // Send up
     *     s.write(curbuf, 100 * sizeof(int), false);
     *     // Use other buffer
     *     swap(curbuf, otherbuf);
     * }
     * delete[] buf1;
     * delete[] buf2;
     * \endcode
     *
     * @remarks Memory is transferred using the `DMA1` engine.
     */
    int write(void* data, size_t size, bool wait_for_completion) {
        size_t remaining = (unsigned)buffer + capacity - (unsigned)cursor;
        if (size > remaining) {
            // TODO: write to host request
            return -1;
        }
        // Wait for any previous transfer to finish (either down or up)
        wait();
        // Round size up to a multiple of 8
        // If this is not done, integer access to the headers will crash
        size = ((size + 8 - 1) / 8) * 8;
        // Write the data (async)
        dma.push(cursor, data, size, 1);
        cursor = (void*)((unsigned)cursor + size);
        if (wait_for_completion)
            wait();
        return size;
    }

    /**
     * Read data from a stream to a local buffer.
     *
     * @param buffer Buffer that receives the data. Must hold at least size.
     * @param wait_for_completion If true this function blocks untill
     * the data is completely read from the stream.
     * @return Number of bytes read. Zero at end of stream, negative on error.
     *
     * The function *always* waits for any previous transfers to finish.
     *
     * @remarks Memory is transferred using the `DMA1` engine.
     */
    int read(void* buffer, size_t size, bool wait_for_completion) {
        size_t remaining = (unsigned)buffer + size - (unsigned)cursor;
        if (remaining == 0) {
            // TODO: read from host request
            return 0;
        }
        if (size > remaining)
            size = remaining;
        wait();
        dma.push(buffer, cursor, size, 1);
        cursor = (void*)((unsigned)cursor + size);
        if (wait_for_completion)
            wait();
        return size;
    }

  private:
    // These are also part of `stream_descriptor`
    void* buffer;
    uint32_t capacity;
    int32_t offset;
    int32_t size;
    // Only local
    __attribute__((aligned(8))) dma_task dma;
    int stream_id;
    void* cursor;
};

} // namespace epiphany
} // namespace bulk