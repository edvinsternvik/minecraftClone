#pragma once
#include <array>
#include "Math.h"

enum BlockSide {
	Front, Back, Left, Right, Top, Down
};

class BlockModelData {
public:
	static std::array<unsigned int, 6> getBlockData(const BlockSide& cubeSide, const Vector3& position, const int& blockId);
	static std::array<unsigned int, 6> getFoliageData(const BlockSide& cubeSide, const Vector3& position, const int& blockId);
};