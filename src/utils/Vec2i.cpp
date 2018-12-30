#include <tuple>

#include <Vec2i.h>

Vec2i::Vec2i(int x, int y) {
    this->x = x;
    this->y = y;
}

std::ostream& operator<<(std::ostream& os, Vec2i const & vec) {
    return os << "(" << vec.x << ", " << vec.y << ")";
}

bool operator==(Vec2i const & a, Vec2i const & b) {
    return a.x == b.x && a.y == b.y;
}

bool operator<(Vec2i const & a, Vec2i const & b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}
