#pragma once
#include <array>
#include "BlockModelData.h"
#include "Math.h"

enum BlockId {
	Air, Grass, Stone, Dirt, Tallgrass
};

enum class BlockType {
	Transparent, Solid
};

class Block {
public:
	Block(BlockId blockId, BlockType blockType, std::array<unsigned int, 6> textureIndicies, float miningSpeed);

	inline BlockId getBlockId() const { return blockId; }
	inline BlockType getBlockType() const { return blockType; }
	inline unsigned int getTextureId(const BlockSide& blockSide) const { 
		return m_textureIndicies[(unsigned int)blockSide];
	}

private:
	const BlockId blockId;
	const BlockType blockType;
	std::array<unsigned int, 6> m_textureIndicies;
	const float miningSpeed;
};

class BlockManager {
public:
	static const Block& getBlock(BlockId& blockId);

public:
	static const unsigned int BlockCount = 5; 
	static const Block Blocks[BlockCount];
};