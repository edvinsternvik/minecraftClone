#include "Block.h"

Block::Block(BlockId blockId, BlockType blockType, std::array<unsigned int, 6> textureIndicies, float miningSpeed) :
    blockId(blockId), blockType(blockType), miningSpeed(miningSpeed) {
        for(int i = 0; i < 6; ++i) {
            m_textureIndicies[i] = textureIndicies[i];
        }
}

const Block BlockManager::Blocks[BlockCount] {
    Block(BlockId::Air, BlockType::None, {0,0,0,0,0,0}, 0.0f),
    Block(BlockId::Grass, BlockType::Opaque, {3,3,3,3,0,2} , 1.0f),
    Block(BlockId::Stone, BlockType::Opaque, {1,1,1,1,1,1}, 1.0f),
    Block(BlockId::Dirt, BlockType::Opaque, {2,2,2,2,2,2}, 2.0f),
    Block(BlockId::Tallgrass, BlockType::Foliage, {4,4,4,4,4,4}, 0.0f),
    Block(BlockId::Wood, BlockType::Opaque, {5,5,5,5,6,6}, 1.0f),
    Block(BlockId::Leaves, BlockType::Transparent, {7,7,7,7,7,7}, 0.1f)
};

const Block& BlockManager::getBlock(const BlockId& blockId) {
    if(blockId < 0 || blockId >= BlockCount) {
        return Blocks[0];
    }

    return Blocks[blockId];
}
