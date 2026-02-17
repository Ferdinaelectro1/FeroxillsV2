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
    void advanceRead(size_t n); //avancer la fenetre d'un pas donné
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

/**
 *
 * @param win_size taille de la fenetre à récupérer
 * @return retourne les win_size élément les plus récents présent dans le ring buffer
 * afin de permettre à ce que l'analyseur analyse l'échantillons récents , et pas
 * des echantillons passés
 */
template<typename T, size_t capacity>
QVector<T> FRingBuf<T,capacity>::getRecentWindows(size_t win_size) {
    QVector<T> out;
    if (win_size == 0) return  out;
    if (win_size > m_size) win_size = m_size; //limite la taille de la fenetre demandé par l'analyseur
    out.reserve(win_size);
    // mémorisation du write index pour éviter des incohérence ou races conditions
    const size_t write_snapshot = m_write_index;

    // Calcul du début en modulo pur
    const size_t start = (write_snapshot + capacity - win_size) % capacity;

    // Nombre d'éléments jusqu'à la fin physique du buffer
    const size_t first_part = std::min(win_size, capacity - start);

    //première partie, de start jusqu'à la fin du buffer
    for (size_t i = 0; i < first_part; ++i)
        out.push_back(m_data[start + i]);

    // deuxième partie, si wrap nécessaire
    const size_t remaining = win_size - first_part;
    for (size_t i = 0; i < remaining; ++i)
        out.push_back(m_data[i]);
    return  out;
}

//À solidifier  plus tard
template<typename T, size_t capacity>
void FRingBuf<T,capacity>::getWindowFromIndex(const unsigned long index, T* out,const size_t windowSize) {
    //la taille demandé est trop énorme
    assert(out != nullptr);
    assert(index + windowSize <= capacity);
    for (unsigned long i = 0; i < windowSize; ++i) {
        out[i] = m_data[i + index];
        qDebug() << " <- "<<out[i];
    }
}