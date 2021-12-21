#include "Chunk.h"
#include "ChunkRenderer.h"
#include "Application.h"

#define CHUNK_WIDTH_INVERSE 1.0 / (float)CHUNK_WIDTH
#define NOISE_OFFSET 10000.0

Chunk::Chunk(int chunkX, int chunkZ, const PerlinNoise& noiseGenerator) : chunkX(chunkX), chunkZ(chunkZ) {
	for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
		int chunkSegmentYPos = i * CHUNK_SEGMENT_HEIGHT;

		for(int x = 0; x < CHUNK_WIDTH; ++x) {
			for(int z = 0; z < CHUNK_WIDTH; ++z) {
				for(int y = 0; y < CHUNK_SEGMENT_HEIGHT; ++y) {
					int yPos = y + chunkSegmentYPos;

					BlockId blockId = BlockId::Air;

					float noiseX = ((float)x * CHUNK_WIDTH_INVERSE + (float)chunkX) + NOISE_OFFSET;
					float noiseZ = ((float)z * CHUNK_WIDTH_INVERSE + (float)chunkZ) + NOISE_OFFSET;

					float noiseValue = 32;
					noiseValue += noiseGenerator.noise(noiseX * 1.0 + NOISE_OFFSET, noiseZ * 1.0 + NOISE_OFFSET) * 4;
					noiseValue += noiseGenerator.noise(noiseX * 0.15, noiseZ * 0.15) * 24;

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

bool Chunk::isSolid(int x, int y, int z) const {
	if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
		return false;
	}

	Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
	return BlockManager::getBlock(blocks[segPos.w][segPos.x][segPos.y][segPos.z]).getBlockType() == BlockType::Solid;
}

Vector4i Chunk::calculateSegmentPosFromChunkPos(int x, int y, int z) const {
	return Vector4i(x, y % CHUNK_SEGMENT_HEIGHT, z, y >> 4);
}
