#pragma once
#include <array>
#include "Math.h"

enum BlockSide {
	Front, Back, Left, Right, Top, Down
};

class BlockModelData {
public:
	static std::array<float, 48> getBlockData(BlockSide cubeSide, Vector3 position, int blockId);
	static std::array<float, 48> getFoliageData(BlockSide cubeSide, Vector3 position, int blockId);
};