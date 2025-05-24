#pragma once
#include "../misc/Math.h"
#include "../world/Block.h"

struct StructureBlock {
    Vector3i pos;
    BlockId block;
};

struct Structures {
    static const StructureBlock tree_structure[66];
};
