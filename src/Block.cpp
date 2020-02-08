#include "Block.h"

Block::Block(BlockId blockId, BlockType blockType, std::array<unsigned int, 12> texCoords, float miningSpeed) : 
	blockId(blockId), blockType(blockType), miningSpeed(miningSpeed) {
		for(int i = 0; i < 12; ++i) {
			m_textureCoords[i] = texCoords[i];
		}
}

const Block BlockManager::Blocks[BlockCount] {
	Block(BlockId::Air ,BlockType::Transparent, {0,0, 0,0, 0,0, 0,0, 0,0, 0,0}, 0.0f), 
	Block(BlockId::Grass ,BlockType::Solid, {3,0, 3,0, 3,0, 3,0, 0,0, 2,0} , 1.0f), 
	Block(BlockId::Stone ,BlockType::Solid, {1,0, 1,0, 1,0, 1,0, 1,0, 1,0}, 1.0f), 
	Block(BlockId::Dirt ,BlockType::Solid, {2,0, 2,0, 2,0, 2,0, 2,0, 2,0}, 2.0f)
};

const Block& BlockManager::getBlock(BlockId& blockId) {
	if(blockId < 0 || blockId >= BlockCount) {
		return Blocks[0];
	}

	return Blocks[blockId];
}

std::array<float, 48> BlockModelData::getData(BlockSide cubeSide, Vector3 position, Vector2i TextureCoords) {
	float textureX = TextureCoords.x, textureY = TextureCoords.y;
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