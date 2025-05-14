#pragma once
#include <array>
#include "../world/Block.h"
#include "../misc/Math.h"

class BlockModelData {
public:
	static std::array<unsigned int, 6> getBlockData(const BlockSide& cubeSide, const Vector3& position, const int& blockId);
	static std::array<unsigned int, 6> getFoliageData(const BlockSide& cubeSide, const Vector3& position, const int& blockId);
};
