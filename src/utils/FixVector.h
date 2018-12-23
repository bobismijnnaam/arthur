#ifndef FIXVECTOR_H
#define FIXVECTOR_H

#include <array>

template<typename T, unsigned int N>
class FixVector {
public:
    std::array<T, N> data;
    int size = 0;

    T& get(int index) {
        return data.at(index);
    }

    T const & get(int index) const {
        return data.at(index);
    }

    void set(int index, T const & elem) {
        data[index] = elem;
    }

    void push(T const & elem) {
        data[size++] = elem;
    }

    T pop() {
        return data[size--];
    }

    T removeIndex(int i) {
        T elem = data[i];
        data[i] = data[size - 1];
        size--;
        return elem;
    }

    void removeElem(T const & t) {
        for (int i = 0; i < size; i++) {
            if (data[i] == t) {
                data[i] = data[size - 1]; 
                size--;
                return;
            }
        }
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return N;
    }

    void clear() {
        size = 0;
    }

    bool contains(T const & elem) const {
        for (int i = 0; i < size; ++i) {
            if (data[i] == elem) {
                return true;
            }
        }

        return false;
    }
} ;

#endif // FIXVECTOR_H
