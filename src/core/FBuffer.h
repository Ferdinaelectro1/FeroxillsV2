#pragma once
#include <QVector>
#include <QDebug>

template<typename T,size_t capacity>
class FRingBuf {

public:
    FRingBuf();
    [[nodiscard]] unsigned long size() const;
    [[nodiscard]] size_t freeSpace() const { return capacity - m_size; }
    void push(T newValue);
    //T read();
    void advanceRead(size_t n); // advance the window by the specified step
    void getWindow(T* out,size_t windowSize);
    [[nodiscard]] QVector<T> getRecentWindows(size_t win_size);
    void getWindowFromIndex(unsigned long index, T* out,size_t windowSize);
    void clear();

private:
    T m_data[capacity] = {0};
    unsigned long m_read_index;
    unsigned long m_write_index;
    unsigned long m_size;
};

template<typename T, size_t capacity>
FRingBuf<T,capacity>::FRingBuf(){
    m_write_index = 0;
    m_read_index = 0;
    m_size = 0;
}

template<typename T, size_t capacity>
unsigned long FRingBuf<T,capacity>::size() const {
    return m_size;
}

template<typename T, size_t capacity>
void FRingBuf<T,capacity>::push(T newValue) {
    m_data[m_write_index] = newValue;
    // optimization trick: reset the write index to 0 when there is no remaining space
    m_write_index = (m_write_index + 1) % capacity;
    if(m_size < capacity){
        ++ m_size;
    }
}

template<typename T, size_t capacity>
void FRingBuf<T,capacity>::getWindow(T* out,size_t windowSize) {
    if (windowSize > m_size)
        windowSize = m_size;
    // start from the current read index to perform the read
    size_t idx = m_read_index;
    for (size_t i = 0; i < windowSize; ++i) {
        out[i] = m_data[idx];
        idx = (idx + 1) % capacity;
    }
}

template<typename T, size_t capacity>
void FRingBuf<T,capacity>::advanceRead(size_t n) {
    if (n > m_size)
        n = m_size;
    m_read_index = (m_read_index + n) % capacity;
    m_size -= n;
}

template<typename T, size_t capacity>
void FRingBuf<T,capacity>::clear() {
    m_read_index = 0;
    m_write_index = 0;
    m_size = 0;
}

/**
 *
 * @param win_size window size to retrieve
 * @return returns the most recent win_size elements currently present in the ring buffer
 * so the analyser can inspect recent samples instead of stale ones
 */
template<typename T, size_t capacity>
QVector<T> FRingBuf<T,capacity>::getRecentWindows(size_t win_size) {
    QVector<T> out;
    if (win_size == 0) return  out;
    if (win_size > m_size) win_size = m_size; // limit the requested window size to the analyser
    out.reserve(win_size);
    // keep a snapshot of the write index to avoid races and inconsistencies
    const size_t write_snapshot = m_write_index;

    // Calculate the start index using pure modulo arithmetic
    const size_t start = (write_snapshot + capacity - win_size) % capacity;

    // Number of elements until the physical end of the buffer
    const size_t first_part = std::min(win_size, capacity - start);

    // first part: from start to the end of the buffer
    for (size_t i = 0; i < first_part; ++i)
        out.push_back(m_data[start + i]);

    // second part: if wraparound is required
    const size_t remaining = win_size - first_part;
    for (size_t i = 0; i < remaining; ++i)
        out.push_back(m_data[i]);
    return  out;
}

// To be hardened later
template<typename T, size_t capacity>
void FRingBuf<T,capacity>::getWindowFromIndex(const unsigned long index, T* out,const size_t windowSize) {
    // requested size is too large
    assert(out != nullptr);
    assert(index + windowSize <= capacity);
    for (unsigned long i = 0; i < windowSize; ++i) {
        out[i] = m_data[i + index];
    }
}