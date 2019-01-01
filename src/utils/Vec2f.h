#ifndef VEC2F_H
#define VEC2F_H

#include <ostream>

struct Vec2f {
    Vec2f() = default;
    Vec2f(float x, float y);

    friend std::ostream& operator<<(std::ostream& os, Vec2f const & vec);
    friend bool operator==(Vec2f const & a, Vec2f const & b);

    float x, y;
} ;

bool operator<(Vec2f const & a, Vec2f const & b);
Vec2f operator+(Vec2f const & a, Vec2f const & b);

#endif // VEC2F_H
