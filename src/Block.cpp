#include "Block.h"

Block::Block(BlockId blockId, BlockType blockType, std::array<unsigned int, 12> texCoords, float miningSpeed) : 
	blockId(blockId), blockType(blockType), miningSpeed(miningSpeed) {
		for(int i = 0; i < 12; ++i) {
			m_textureCoords[i] = texCoords[i];
		}
}

const Block BlockManager::Blocks[BlockCount] {
	Block(BlockId::Air, BlockType::Transparent, {0,0, 0,0, 0,0, 0,0, 0,0, 0,0}, 0.0f), 
	Block(BlockId::Grass, BlockType::Solid, {3,0, 3,0, 3,0, 3,0, 0,0, 2,0} , 1.0f), 
	Block(BlockId::Stone, BlockType::Solid, {1,0, 1,0, 1,0, 1,0, 1,0, 1,0}, 1.0f), 
	Block(BlockId::Dirt, BlockType::Solid, {2,0, 2,0, 2,0, 2,0, 2,0, 2,0}, 2.0f),
	Block(BlockId::Tallgrass, BlockType::Transparent, {4,0, 0,0, 0,0, 0,0, 0,0, 0,0}, 0.0f)
};

const Block& BlockManager::getBlock(BlockId& blockId) {
	if(blockId < 0 || blockId >= BlockCount) {
		return Blocks[0];
	}

	return Blocks[blockId];
}