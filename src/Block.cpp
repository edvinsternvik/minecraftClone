#include "Block.h"

Block::Block(BlockId blockId, BlockType blockType, unsigned int textureX, unsigned int textureY, float miningSpeed) : 
	blockId(blockId), blockType(blockType), textureX(textureX), textureY(textureY), miningSpeed(miningSpeed) {}

const Block BlockManager::Blocks[BlockCount] {
	Block(BlockId::Air ,BlockType::Transparent, 0, 0, 0.0f), 
	Block(BlockId::Grass ,BlockType::Solid, 0, 0, 1.0f), 
	Block(BlockId::Stone ,BlockType::Solid, 1, 0, 1.0f), 
	Block(BlockId::Dirt ,BlockType::Solid, 2, 0, 2.0f)
};

const Block& BlockManager::getBlock(BlockId& blockId) {
	if(blockId < 0 || blockId >= BlockCount) {
		return Blocks[0];
	}

	return Blocks[blockId];
}