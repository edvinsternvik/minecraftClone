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
	Block(BlockId blockId, BlockType blockType, std::array<unsigned int, 12> texCoords, float miningSpeed);

	inline BlockId getBlockId() const { return blockId; }
	inline BlockType getBlockType() const { return blockType; }
	inline Vector2i getTexture(BlockSide blockSide) const { 
		int index = (int)blockSide * 2;
		return Vector2i(m_textureCoords[index], m_textureCoords[index + 1]);
	}
	inline int getTextureX(BlockSide blockSide) const { return m_textureCoords[((int)blockSide) * 2]; }
	inline int getTextureY(BlockSide blockSide) const { return m_textureCoords[((int)blockSide) * 2 + 1]; }

private:
	const BlockId blockId;
	const BlockType blockType;
	std::array<unsigned int, 12> m_textureCoords;
	const float miningSpeed;
};

class BlockManager {
public:
	static const Block& getBlock(BlockId& blockId);

public:
	static const unsigned int BlockCount = 5; 
	static const Block Blocks[BlockCount];
};