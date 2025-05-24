#include "Structures.h"

const StructureBlock Structures::tree_structure[66] = {
    {{0, 0, 0}, BlockId::Wood},     {{0, 1, 0}, BlockId::Wood},
    {{0, 2, 0}, BlockId::Wood},     {{0, 3, 0}, BlockId::Wood},

    {{-2, 2, -2}, BlockId::Leaves}, {{-1, 2, -2}, BlockId::Leaves},
    {{0, 2, -2}, BlockId::Leaves},  {{1, 2, -2}, BlockId::Leaves},
    {{2, 2, -2}, BlockId::Leaves},  {{-2, 2, -1}, BlockId::Leaves},
    {{-1, 2, -1}, BlockId::Leaves}, {{0, 2, -1}, BlockId::Leaves},
    {{1, 2, -1}, BlockId::Leaves},  {{2, 2, -1}, BlockId::Leaves},
    {{-2, 2, 0}, BlockId::Leaves},  {{-1, 2, 0}, BlockId::Leaves},
    {{1, 2, 0}, BlockId::Leaves},   {{2, 2, 0}, BlockId::Leaves},
    {{-2, 2, 1}, BlockId::Leaves},  {{-1, 2, 1}, BlockId::Leaves},
    {{0, 2, 1}, BlockId::Leaves},   {{1, 2, 1}, BlockId::Leaves},
    {{2, 2, 1}, BlockId::Leaves},   {{-2, 2, 2}, BlockId::Leaves},
    {{-1, 2, 2}, BlockId::Leaves},  {{0, 2, 2}, BlockId::Leaves},
    {{1, 2, 2}, BlockId::Leaves},   {{2, 2, 2}, BlockId::Leaves},

    {{-2, 3, -2}, BlockId::Leaves}, {{-1, 3, -2}, BlockId::Leaves},
    {{0, 3, -2}, BlockId::Leaves},  {{1, 3, -2}, BlockId::Leaves},
    {{2, 3, -2}, BlockId::Leaves},  {{-2, 3, -1}, BlockId::Leaves},
    {{-1, 3, -1}, BlockId::Leaves}, {{0, 3, -1}, BlockId::Leaves},
    {{1, 3, -1}, BlockId::Leaves},  {{2, 3, -1}, BlockId::Leaves},
    {{-2, 3, 0}, BlockId::Leaves},  {{-1, 3, 0}, BlockId::Leaves},
    {{1, 3, 0}, BlockId::Leaves},   {{2, 3, 0}, BlockId::Leaves},
    {{-2, 3, 1}, BlockId::Leaves},  {{-1, 3, 1}, BlockId::Leaves},
    {{0, 3, 1}, BlockId::Leaves},   {{1, 3, 1}, BlockId::Leaves},
    {{2, 3, 1}, BlockId::Leaves},   {{-2, 3, 2}, BlockId::Leaves},
    {{-1, 3, 2}, BlockId::Leaves},  {{0, 3, 2}, BlockId::Leaves},
    {{1, 3, 2}, BlockId::Leaves},   {{2, 3, 2}, BlockId::Leaves},

    {{-1, 4, -1}, BlockId::Leaves}, {{0, 4, -1}, BlockId::Leaves},
    {{1, 4, -1}, BlockId::Leaves},  {{-1, 4, 0}, BlockId::Leaves},
    {{0, 4, 0}, BlockId::Leaves},   {{1, 4, 0}, BlockId::Leaves},
    {{-1, 4, 1}, BlockId::Leaves},  {{0, 4, 1}, BlockId::Leaves},
    {{1, 4, 1}, BlockId::Leaves},

    {{-1, 5, 0}, BlockId::Leaves},  {{1, 5, 0}, BlockId::Leaves},
    {{0, 5, 0}, BlockId::Leaves},   {{0, 5, -1}, BlockId::Leaves},
    {{0, 5, 1}, BlockId::Leaves},
};
