#include <unordered_map>
#include <string>

struct Parameter {
    uint16_t address;
    uint8_t width;
    uint8_t shift;

    // Constructor to simplify initialization
    Parameter(uint16_t addr, uint8_t w = 0, uint8_t s = 0)
        : address(addr), width(w), shift(s) {}
};