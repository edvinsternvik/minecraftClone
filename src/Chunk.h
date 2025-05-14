#pragma once
#include "Block.h"
#include "Math.h"
#include <memory>
#include "PerlinNoise.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 128
#define CHUNK_SEGMENT_HEIGHT CHUNK_WIDTH
#define CHUNK_SEGMENTS CHUNK_HEIGHT / CHUNK_SEGMENT_HEIGHT

class ChunkRenderer;

class Chunk {
public:
	Chunk(int chunkX, int chunkZ, const PerlinNoise& noiseGenerator);
	~Chunk();

	bool isSolid(int x, int y, int z) const;
	inline const Block& getBlock(int x, int y, int z) const {
		Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
		return BlockManager::getBlock(blocks[segPos.w][segPos.x][segPos.y][segPos.z]);
	}
	void changeBlock(int x, int y, int z, BlockId blockId);
	Vector4i calculateSegmentPosFromChunkPos(int x, int y, int z) const;
	inline ChunkRenderer* const getChunkRenderer() const { return m_chunkRenderer.get(); }

public:
	const int chunkX, chunkZ;

private:
	BlockId blocks[CHUNK_SEGMENTS][CHUNK_WIDTH][CHUNK_SEGMENT_HEIGHT][CHUNK_WIDTH];
	std::unique_ptr<ChunkRenderer> m_chunkRenderer;


	friend class World;
};
