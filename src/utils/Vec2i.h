#ifndef VEC2I_H
#define VEC2I_H

#include <ostream>

struct Vec2i {
    Vec2i() = default;
    Vec2i(int x, int y);

    friend std::ostream& operator<<(std::ostream& os, Vec2i const & vec);
    friend bool operator==(Vec2i const & a, Vec2i const & b);

    int x, y;
} ;

bool operator<(Vec2i const & a, Vec2i const & b);

#endif // VEC2I_H
