#include "Chunk.h"
#include "Application.h"

#include <iostream>
Chunk::Chunk(int chunkX, int chunkZ) : chunkX(chunkX), chunkZ(chunkZ), next(nullptr), chunkMeshSize(0) {
	for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
		for(int x = 0; x < CHUNK_WIDTH; ++x) {
			for(int y = 0; y < CHUNK_SEGMENT_HEIGHT; ++y) {
				for(int z = 0; z < CHUNK_WIDTH; ++z) {
					BlockId blockId = BlockId::Air;

					if(i < 4) {
						blockId = BlockId::Dirt;
					}

					blocks[i][x][y][z] = blockId;
				}
			}	
		}
	}

	// blocks[3][CHUNK_HEIGHT - 1][3] = BlockId::Air;
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
		updateChunkMesh(x, y, z);
	}

}

bool Chunk::isSolid(int x, int y, int z) {
	if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
		return false;
	}

	Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
	return BlockManager::getBlock(blocks[segPos.w][segPos.x][segPos.y][segPos.z]).getBlockType() == BlockType::Solid;
}

void Chunk::generateChunkMesh() {
	for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
		generateChunkSegmentMesh(i);
	}

	chunkMeshSize = 0;
	for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
		chunkMeshSize += getChunkSegmentMeshVerticiesCount(i);
	}
}

void Chunk::generateChunkSegmentMesh(int index){
	if(index < 0 || index >= CHUNK_SEGMENTS) return;

	auto* segmentMesh = &chunkMesh[index];

	segmentMesh->resize(0);
	int chunkWorldX = chunkX * CHUNK_WIDTH;
	int chunkWorldZ = chunkZ * CHUNK_WIDTH;

	World* world = Application::getInstance().getWorld();

	int yOffset = index * CHUNK_SEGMENT_HEIGHT;
	for(int x = 0; x < CHUNK_WIDTH; ++x) {
		for(int y = yOffset; y < CHUNK_SEGMENT_HEIGHT + yOffset; ++y) {
			for(int z = 0; z < CHUNK_WIDTH; ++z) {
				const Block* currentBlock = &getBlock(x, y, z);
				if(currentBlock->getBlockType() != BlockType::Transparent) {
					// std::cout << x << ", " << y << ", " << z << std::endl;
				}
				if(currentBlock->getBlockType() == BlockType::Solid) {
					int worldX = x + chunkWorldX, worldZ = z + chunkWorldZ;

					int textureX = currentBlock->getTextureX();
					int textureY = currentBlock->getTextureY();
					if(!world->isSolid(worldX, y, worldZ + 1)) segmentMesh->push_back(CubeModelData::getData(CubeSide::Front, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX, y, worldZ - 1)) segmentMesh->push_back(CubeModelData::getData(CubeSide::Back, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX - 1, y, worldZ)) segmentMesh->push_back(CubeModelData::getData(CubeSide::Left, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX + 1, y, worldZ)) segmentMesh->push_back(CubeModelData::getData(CubeSide::Right, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX, y + 1, worldZ)) segmentMesh->push_back(CubeModelData::getData(CubeSide::Top, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX, y - 1, worldZ)) segmentMesh->push_back(CubeModelData::getData(CubeSide::Down, Vector3(worldX, y, worldZ), textureX, textureY));
				}
			}
		}
	}

	chunkMeshSize = 0;
	for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
		chunkMeshSize += getChunkSegmentMeshVerticiesCount(i);
	}
}

void Chunk::updateChunkMesh(int x, int y, int z){
	Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
	generateChunkSegmentMesh(segPos.w);

	int chunkSegmentY = y % CHUNK_SEGMENT_HEIGHT;
	if(chunkSegmentY == 0 && segPos.w > 0) generateChunkSegmentMesh(segPos.w - 1);
	if(chunkSegmentY == CHUNK_SEGMENT_HEIGHT - 1 && segPos.w < CHUNK_SEGMENTS - 1) generateChunkSegmentMesh(segPos.w + 1); 
}

Vector4i Chunk::calculateSegmentPosFromChunkPos(int x, int y, int z) {
	return Vector4i(x, y % CHUNK_SEGMENT_HEIGHT, z, std::floor((float)y / (float)CHUNK_SEGMENT_HEIGHT));
}