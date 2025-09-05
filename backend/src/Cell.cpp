#include "Cell.hpp"

std::ostream& operator<<(std::ostream& os, Cell cell) {
    switch (cell) {
        case Cell::Empty: return os << ".";
        case Cell::Black: return os << "B";
        case Cell::White: return os << "W";
    }
    return os << "Unknown"; // fallback, should never happen
}
