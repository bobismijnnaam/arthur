#include <array>

// Needs:
// - A makeMove functoin that also updates the neutral array
// - an isQuadrantNeutral function 
// - an isQuadrantSymmetric function that besides neutrality also checks if
//   there is symmetry
// - a five in a row checking function that takes one position (in the case
//   that only one position changed)
// - a general (slow) 5 in a row checking function that checks everything
// - a five in a row checking function that takes a quadrant

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

int const BOARD_WIDTH = 6;
int const BOARD_HEIGHT = 6;
int const NUM_POSITIONS = BOARD_WIDTH * BOARD_HEIGHT;
int const MAX_NUM_PIECES_PER_COLOR = 3 * 6;
int const NUM_QUADRANTS = 4;

struct Vec2I {
    Vec2I() = default;
    Vec2I(int x, int y);

    int x, y;
} ;

Vec2I::Vec2I(int x, int y) {
    this.x = x;
    this.y = y;
}

enum class PositionState : uint8_t {
    EMPTY,
    BLACK,
    WHITE
} ;

enum class Quadrant : uint8_t {
    TOP_LEFT = 0,
    TOP_RIGHT = 1,
    BOTTOM_LEFT = 2,
    BOTTOM_RIGHT = 3
} ;

class Pentago {
public:
    void clear();
    
    void getEmptyPositions(FixVector<Vec2I, NUM_POSITIONS>& res);
    void rotateQuadrant(Quadrant const quadrant, bool const clockwise);

    void getPositionState(Vec2I const pos);
    void setPositionState(Vec2I const pos, PositionState const newState);

private:
    std::array<PositionState, NUM_POSITIONS> positions;
    int numPiecesBlack;
    int numPiecesWhite;
    
    std::array<bool, NUM_QUADRANTS> neutralQuadrants;
} ;

void Pentago::clear() {
    for (int i = 0; i < NUM_POSITIONS; ++i) {
        positions[i] = PositionState.EMPTY;
    }

    numPiecesBlack = MAX_NUM_PIECES_PER_COLOR;
    numPiecesWhite = MAX_NUM_PIECES_PER_COLOR;
}

void Pentago::getEmptyPositions(FixVector<Vec2I, NUM_POSITIONS>& res) {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            Vec2I pos(x, y);
            if (getPositionState(pos) == PositionState.EMPTY) {
                res.push(pos);
            }
        }
    }
}

void rotateQuadrant(Quadrant const quadrant, bool const clockwise) {
    
}

void getPositionState(Vec2I const pos) {
    int i = pos.y * BOARD_WIDTH + pos.x;
    return positions[i];
}

void Pentago::setPositionState(Vec2I const pos, PositionState newState) {
    positions[pos.y * BOARD_WIDTH + pos.x] = newState;
}
