#pragma once

enum BlockId {
	Air, Grass, Stone, Dirt
};

enum class BlockType {
	Transparent, Solid
};

class Block {
public:
	Block(BlockId blockId, BlockType blockType, unsigned int textureX, unsigned int textureY, float miningSpeed);

	inline BlockType getBlockType() const { return blockType; }
	inline int getTextureX() const { return textureX; }
	inline int getTextureY() const { return textureY; }

private:
	const BlockId blockId;
	const BlockType blockType;
	const unsigned int textureX, textureY;
	const float miningSpeed;
};

class BlockManager {
public:
	static const Block& getBlock(BlockId& blockId);

public:
	static const unsigned int BlockCount = 4; 
	static const Block Blocks[BlockCount];
};

// class Block {
// public:
// 	static virtual inline const char* getBlockName(Block block) = 0;
// 	static virtual inline int getBlockId(Block block);
// 	static virtual inline BlockType getBlockType(Block block);
// 	//getBlockTextureOffset
// };