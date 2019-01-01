#include <tuple>

#include "Vec2f.h"

Vec2f::Vec2f(float x, float y) {
    this->x = x;
    this->y = y;
}

std::ostream& operator<<(std::ostream& os, Vec2f const & vec) {
    return os << "(" << vec.x << ", " << vec.y << ")";
}

bool operator==(Vec2f const & a, Vec2f const & b) {
    return a.x == b.x && a.y == b.y;
}

Vec2f operator+(Vec2f const & a, Vec2f const & b) {
    return Vec2f(a.x + b.x, a.y + b.y);
}

bool operator<(Vec2f const & a, Vec2f const & b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}
