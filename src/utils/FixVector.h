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

    void set(int index, T const & elem) {
        data[index] = elem;
    }

    void push(T const & elem) {
        data[size++] = elem;
    }

    T pop() {
        return data[size--];
    }

    int getSize() {
        return size;
    }

    int getCapacity() {
        return N;
    }

    void clear() {
        size = 0;
    }

    bool contains(T const & elem) {
        for (int i = 0; i < size; ++i) {
            if (data[i] == elem) {
                return true;
            }
        }

        return false;
    }
} ;

#endif // FIXVECTOR_H
