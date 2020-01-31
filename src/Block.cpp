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
	int textureX = TextureCoords.x, textureY = TextureCoords.y;
	if(cubeSide == BlockSide::Front) {
		return {
			0.0f  + position.x, 0.0f  + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 0.0f + textureX, 0.0f + textureY,
			1.0f + position.x, 0.0f  + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 1.0f + textureX, 0.0f + textureY,
			1.0f + position.x,  1.0f + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 1.0f + textureX, 1.0f + textureY,

			1.0f + position.x,  1.0f + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 1.0f + textureX, 1.0f + textureY,
			0.0f  + position.x,  1.0f + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 0.0f + textureX, 1.0f + textureY,
			0.0f  + position.x, 0.0f  + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 0.0f + textureX, 0.0f + textureY
		};
	}
	else if(cubeSide == BlockSide::Back) {
		return {
			1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 1.0f + textureX, 1.0f + textureY,
			1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 1.0f + textureX, 0.0f + textureY,
			0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 0.0f + textureX, 0.0f + textureY,

			0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 0.0f + textureX, 0.0f + textureY,
			0.0f  + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 0.0f + textureX, 1.0f + textureY,
			1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 1.0f + textureX, 1.0f + textureY
		};
	}
	else if(cubeSide == BlockSide::Left) {
		return {
			0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, -1.0, 0.0, 0.0, 0.0f + textureX, 0.0f + textureY,
			0.0f  + position.x, 0.0f  + position.y,  1.0f + position.z, -1.0, 0.0, 0.0, 1.0f + textureX, 0.0f + textureY,
			0.0f  + position.x,  1.0f + position.y,  1.0f + position.z, -1.0, 0.0, 0.0, 1.0f + textureX, 1.0f + textureY,

			0.0f  + position.x,  1.0f + position.y,  1.0f + position.z, -1.0, 0.0, 0.0, 1.0f + textureX, 1.0f + textureY,
			0.0f  + position.x,  1.0f + position.y, 0.0f  + position.z, -1.0, 0.0, 0.0, 0.0f + textureX, 1.0f + textureY,
			0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, -1.0, 0.0, 0.0, 0.0f + textureX, 0.0f + textureY
		};
	}
	else if(cubeSide == BlockSide::Right) {
		return {
			1.0f + position.x,  1.0f + position.y,  1.0f + position.z, 1.0, 0.0, 0.0, 1.0f + textureX, 1.0f + textureY,
			1.0f + position.x, 0.0f  + position.y,  1.0f + position.z, 1.0, 0.0, 0.0, 1.0f + textureX, 0.0f + textureY,
			1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 1.0, 0.0, 0.0, 0.0f + textureX, 0.0f + textureY,

			1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 1.0, 0.0, 0.0, 0.0f + textureX, 0.0f + textureY,
			1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 1.0, 0.0, 0.0, 0.0f + textureX, 1.0f + textureY,
			1.0f + position.x,  1.0f + position.y,  1.0f + position.z, 1.0, 0.0, 0.0, 1.0f + textureX, 1.0f + textureY
		};
	}
	else if(cubeSide == BlockSide::Top) {
		return {
			0.0f  + position.x,  1.0f + position.y,  1.0f + position.z, 0.0, 1.0, 0.0, 0.0f + textureX, 0.0f + textureY,
			1.0f + position.x,  1.0f + position.y,  1.0f + position.z, 0.0, 1.0, 0.0, 1.0f + textureX, 0.0f + textureY,
			1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 1.0, 0.0, 1.0f + textureX, 1.0f + textureY,

			1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 1.0, 0.0, 1.0f + textureX, 1.0f + textureY,
			0.0f  + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 1.0, 0.0, 0.0f + textureX, 1.0f + textureY,
			0.0f  + position.x,  1.0f + position.y,  1.0f + position.z, 0.0, 1.0, 0.0, 0.0f + textureX, 0.0f + textureY
		};
	}
	else {
		return {
			1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, -1.0, 0.0, 1.0f + textureX, 1.0f + textureY,
			1.0f + position.x, 0.0f  + position.y,  1.0f + position.z, 0.0, -1.0, 0.0, 1.0f + textureX, 0.0f + textureY,
			0.0f  + position.x, 0.0f  + position.y,  1.0f + position.z, 0.0, -1.0, 0.0, 0.0f + textureX, 0.0f + textureY,

			0.0f  + position.x, 0.0f  + position.y,  1.0f + position.z, 0.0, -1.0, 0.0, 0.0f + textureX, 0.0f + textureY,
			0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, -1.0, 0.0, 0.0f + textureX, 1.0f + textureY,
			1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, -1.0, 0.0, 1.0f + textureX, 1.0f + textureY
		};
	}
}