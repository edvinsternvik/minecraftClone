#include "Chunk.h"
#include "ChunkRenderer.h"
#include "Application.h"
#include "PerlinNoise.h"

#include <iostream>
Chunk::Chunk(int chunkX, int chunkZ) : chunkX(chunkX), chunkZ(chunkZ), next(nullptr) {
	PerlinNoise noise;

	for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
		int chunkSegmentYPos = i * CHUNK_SEGMENT_HEIGHT;

		for(int x = 0; x < CHUNK_WIDTH; ++x) {
			for(int z = 0; z < CHUNK_WIDTH; ++z) {
				for(int y = 0; y < CHUNK_SEGMENT_HEIGHT; ++y) {
					int yPos = y + chunkSegmentYPos;

					BlockId blockId = BlockId::Air;

					float noiseX = ((float)x / (float)CHUNK_WIDTH + (float)chunkX) * 0.5;
					float noiseZ = ((float)z / (float)CHUNK_WIDTH + (float)chunkZ) * 0.5;
					float noiseValue = noise.noise(noiseX, noiseZ);
					noiseValue = (noiseValue) * 16 + 64;
					if(yPos < noiseValue) blockId = BlockId::Stone;
					else if(yPos < noiseValue + 3) blockId = BlockId::Dirt;
					else if(yPos < noiseValue + 4) blockId = BlockId::Grass;
					else if(yPos < noiseValue + 5 && rand() % 10 == 0 ) blockId = BlockId::Tallgrass;

					blocks[i][x][y][z] = blockId;
				}
			}	
		}

		m_chunkRenderer = std::make_unique<ChunkRenderer>(this);
	}
}

Chunk::~Chunk() {
}

void Chunk::changeBlock(int x, int y, int z, BlockId blockId) {
	if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
		return;
	}

	Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
	if(y >= 0 && y < CHUNK_HEIGHT && blocks[segPos.w][segPos.x][segPos.y][segPos.z] != blockId) {
		blocks[segPos.w][segPos.x][segPos.y][segPos.z] = blockId;
		m_chunkRenderer->updateChunkMesh(x, y, z);
	}

}

bool Chunk::isSolid(int x, int y, int z) {
	if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
		return false;
	}

	Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
	return BlockManager::getBlock(blocks[segPos.w][segPos.x][segPos.y][segPos.z]).getBlockType() == BlockType::Solid;
}

Vector4i Chunk::calculateSegmentPosFromChunkPos(int x, int y, int z) {
	return Vector4i(x, y % CHUNK_SEGMENT_HEIGHT, z, std::floor((float)y / (float)CHUNK_SEGMENT_HEIGHT));
}