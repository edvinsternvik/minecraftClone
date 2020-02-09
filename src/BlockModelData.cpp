#include "BlockModelData.h"
#include "Block.h"

std::array<float, 48> BlockModelData::getBlockData(BlockSide cubeSide, Vector3 position, int blockId) {
	BlockId id = (BlockId)blockId;
	Block block = BlockManager::getBlock(id);

	Vector2i textureCoords = block.getTexture(cubeSide);
	float textureX = textureCoords.x, textureY = textureCoords.y;
	float tBegin = 0.001, tEnd = 0.999; // Texture offsets
	float pBegin = 0.0, pEnd = 1.0; // Position offsets
	if(cubeSide == BlockSide::Front) {
		return {
			pBegin + position.x, pBegin + position.y, pEnd + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY,
			pEnd   + position.x, pBegin + position.y, pEnd + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tBegin + textureY,
			pEnd   + position.x, pEnd   + position.y, pEnd + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,

			pEnd   + position.x, pEnd   + position.y, pEnd + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,
			pBegin + position.x, pEnd   + position.y, pEnd + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tEnd   + textureY,
			pBegin + position.x, pBegin + position.y, pEnd + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY
		};
	}
	else if(cubeSide == BlockSide::Back) {
		return {
			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, -1.0, tEnd   + textureX, tEnd   + textureY,
			pEnd   + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, -1.0, tEnd   + textureX, tBegin + textureY,
			pBegin + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, -1.0, tBegin + textureX, tBegin + textureY,

			pBegin + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, -1.0, tBegin + textureX, tBegin + textureY,
			pBegin + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, -1.0, tBegin + textureX, tEnd   + textureY,
			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, -1.0, tEnd   + textureX, tEnd   + textureY
		};
	}
	else if(cubeSide == BlockSide::Left) {
		return {
			pBegin + position.x, pBegin + position.y, pBegin + position.z, -1.0, 0.0, 0.0, tBegin + textureX, tBegin + textureY,
			pBegin + position.x, pBegin + position.y, pEnd   + position.z, -1.0, 0.0, 0.0, tEnd   + textureX, tBegin + textureY,
			pBegin + position.x, pEnd   + position.y, pEnd   + position.z, -1.0, 0.0, 0.0, tEnd + textureX, tEnd   + textureY,

			pBegin + position.x, pEnd   + position.y, pEnd   + position.z, -1.0, 0.0, 0.0, tEnd   + textureX, tEnd   + textureY,
			pBegin + position.x, pEnd   + position.y, pBegin + position.z, -1.0, 0.0, 0.0, tBegin + textureX, tEnd   + textureY,
			pBegin + position.x, pBegin + position.y, pBegin + position.z, -1.0, 0.0, 0.0, tBegin + textureX, tBegin + textureY
		};
	}
	else if(cubeSide == BlockSide::Right) {
		return {
			pEnd   + position.x, pEnd   + position.y, pEnd   + position.z, 1.0, 0.0, 0.0, tEnd   + textureX, tEnd   + textureY,
			pEnd   + position.x, pBegin + position.y, pEnd   + position.z, 1.0, 0.0, 0.0, tEnd   + textureX, tBegin + textureY,
			pEnd   + position.x, pBegin + position.y, pBegin + position.z, 1.0, 0.0, 0.0, tBegin + textureX, tBegin + textureY,

			pEnd   + position.x, pBegin + position.y, pBegin + position.z, 1.0, 0.0, 0.0, tBegin + textureX, tBegin + textureY,
			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 1.0, 0.0, 0.0, tBegin + textureX, tEnd   + textureY,
			pEnd   + position.x, pEnd   + position.y, pEnd   + position.z, 1.0, 0.0, 0.0, 1.0f + textureX, tEnd   + textureY
		};
	}
	else if(cubeSide == BlockSide::Top) {
		return {
			pBegin + position.x, pEnd   + position.y, pEnd   + position.z, 0.0, 1.0, 0.0, tBegin + textureX, tBegin + textureY,
			pEnd   + position.x, pEnd   + position.y, pEnd   + position.z, 0.0, 1.0, 0.0, tEnd   + textureX, tBegin + textureY,
			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 1.0, 0.0, tEnd   + textureX, tEnd   + textureY,

			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 1.0, 0.0, tEnd   + textureX, tEnd   + textureY,
			pBegin + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 1.0, 0.0, tBegin + textureX, tEnd   + textureY,
			pBegin + position.x, pEnd   + position.y, pEnd   + position.z, 0.0, 1.0, 0.0, tBegin + textureX, tBegin + textureY
		};
	}
	else {
		return {
			pEnd   + position.x, pBegin + position.y, pBegin + position.z, 0.0, -1.0, 0.0, tEnd   + textureX, tEnd   + textureY,
			pEnd   + position.x, pBegin + position.y, pEnd   + position.z, 0.0, -1.0, 0.0, tEnd   + textureX, tBegin + textureY,
			pBegin + position.x, pBegin + position.y, pEnd   + position.z, 0.0, -1.0, 0.0, tBegin + textureX, tBegin + textureY,

			pBegin + position.x, pBegin + position.y, pEnd   + position.z, 0.0, -1.0, 0.0, tBegin + textureX, tBegin + textureY,
			pBegin + position.x, pBegin + position.y, pBegin + position.z, 0.0, -1.0, 0.0, tBegin + textureX, tEnd   + textureY,
			pEnd   + position.x, pBegin + position.y, pBegin + position.z, 0.0, -1.0, 0.0, tEnd   + textureX, tEnd   + textureY
		};
	}
}

std::array<float, 48> BlockModelData::getFoliageData(BlockSide cubeSide, Vector3 position, int blockId) {
	BlockId id = (BlockId)blockId;
	Block block = BlockManager::getBlock(id);

	Vector2i textureCoords = block.getTexture((BlockSide)0);
	float textureX = textureCoords.x, textureY = textureCoords.y;

	float tBegin = 0.001, tEnd = 0.999; // Texture offsets
	float pBegin = 0.0, pEnd = 1.0; // Position offsets

	if(cubeSide == BlockSide::Front) {
		return {
			pBegin + position.x, pBegin + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY,
			pEnd   + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tBegin + textureY,
			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,

			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,
			pBegin + position.x, pEnd   + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tEnd   + textureY,
			pBegin + position.x, pBegin + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY
		};
	}
	else if(cubeSide == BlockSide::Right) {
		return {
			pBegin + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY,
			pEnd   + position.x, pBegin + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tBegin + textureY,
			pEnd   + position.x, pEnd   + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,

			pEnd   + position.x, pEnd   + position.y, pEnd  + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,
			pBegin + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tEnd   + textureY,
			pBegin + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY
		};
	}
	else if(cubeSide == BlockSide::Back) {
		return {
			pBegin + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY,
			pBegin + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tEnd   + textureY,
			pEnd   + position.x, pEnd   + position.y, pEnd  + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,

			pEnd   + position.x, pEnd   + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,
			pEnd   + position.x, pBegin + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tBegin + textureY,
			pBegin + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY

		};
	}
	else {
		return {
			pBegin + position.x, pBegin + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY,
			pBegin + position.x, pEnd   + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tEnd   + textureY,
			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,

			pEnd   + position.x, pEnd   + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tEnd   + textureY,
			pEnd   + position.x, pBegin + position.y, pBegin + position.z, 0.0, 0.0, 1.0, tEnd   + textureX, tBegin + textureY,
			pBegin + position.x, pBegin + position.y, pEnd   + position.z, 0.0, 0.0, 1.0, tBegin + textureX, tBegin + textureY

		};
	}

}