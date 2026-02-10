#pragma once
#include <cstddef>

template<typename T,size_t capacity>
class FRingBuf {

public:
    FRingBuf();
    [[nodiscard]] unsigned long size() const;
    [[nodiscard]] size_t freeSpace() const { return capacity - m_size; }
    void push(T newValue);
    //T read();
    void advanceRead(size_t n); //avancer la fenetre d'un pas donné
    void getWindow(T* out,size_t windowSize);
    void clear();

private:
    T m_data[capacity];
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
    //astuce pour aller vite (l'index de write est remise à 0, si plus d'espace)
    m_write_index = (m_write_index + 1) % capacity;
    if(m_size < capacity){
        ++ m_size;
    }
}

template<typename T, size_t capacity>
void FRingBuf<T,capacity>::getWindow(T* out,size_t windowSize) {
    if (windowSize > m_size)
        windowSize = m_size;
    //on paret de l'index de lecture courante pour faire notre lecture
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
