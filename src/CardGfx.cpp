#include "CardGfx.h"

const char* CardGfx::SuitStr() {
    switch (_eSuit) {
        case BASTONI:
            return "Bastoni";
        case COPPE:
            return "Coppe";
        case DENARI:
            return "Denari";
        case SPADE:
            return "Spade";
        default:
            break;
    }
    return "";
}