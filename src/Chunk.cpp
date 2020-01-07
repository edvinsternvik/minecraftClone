#include "Chunk.h"
#include "Application.h"

#include <iostream>
Chunk::Chunk(int chunkX, int chunkZ) : chunkX(chunkX), chunkZ(chunkZ), next(nullptr) {
	for(int x = 0; x < CHUNK_WIDTH; ++x) {
		for(int y = 0; y < CHUNK_HEIGHT; ++y) {
			for(int z = 0; z < CHUNK_WIDTH; ++z) {
				blocks[x][y][z] = BlockId::Grass;
				if(y < 63) blocks[x][y][z] = BlockId::Dirt;
				if(y < 60) blocks[x][y][z] = BlockId::Stone;
			}
		}	
	}

	blocks[3][CHUNK_HEIGHT - 1][3] = BlockId::Air;
}

Chunk::~Chunk() {
}

void Chunk::changeBlock(int x, int y, int z, BlockId blockId) {
	if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
		return;
	}

	if(y > 0 && y < CHUNK_HEIGHT && blocks[x][y][z] != blockId) {
		blocks[x][y][z] = blockId;
		generateChunkMesh();
	}

}

bool Chunk::isSolid(int x, int y, int z) {
	if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
		return false;
	}

	return BlockManager::getBlock(blocks[x][y][z]).getBlockType() == BlockType::Solid;
}

void Chunk::generateChunkMesh() {
	chunkMesh.resize(0);
	int chunkWorldX = chunkX * CHUNK_WIDTH;
	int chunkWorldZ = chunkZ * CHUNK_WIDTH;

	World* world = Application::getInstance().getWorld();

	for(int x = 0; x < CHUNK_WIDTH; ++x) {
		for(int y = 0; y < CHUNK_HEIGHT; ++y) {
			for(int z = 0; z < CHUNK_WIDTH; ++z) {
				const Block* currentBlock = &getBlock(x, y, z);
				if(currentBlock->getBlockType() == BlockType::Solid) {
					int worldX = x + chunkWorldX, worldZ = z + chunkWorldZ;

					int textureX = currentBlock->getTextureX();
					int textureY = currentBlock->getTextureY();
					if(!world->isSolid(worldX, y, worldZ + 1)) chunkMesh.push_back(CubeModelData::getData(CubeSide::Front, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX, y, worldZ - 1)) chunkMesh.push_back(CubeModelData::getData(CubeSide::Back, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX - 1, y, worldZ)) chunkMesh.push_back(CubeModelData::getData(CubeSide::Left, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX + 1, y, worldZ)) chunkMesh.push_back(CubeModelData::getData(CubeSide::Right, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX, y + 1, worldZ)) chunkMesh.push_back(CubeModelData::getData(CubeSide::Top, Vector3(worldX, y, worldZ), textureX, textureY));
					if(!world->isSolid(worldX, y - 1, worldZ)) chunkMesh.push_back(CubeModelData::getData(CubeSide::Down, Vector3(worldX, y, worldZ), textureX, textureY));
				}
			}
		}
	}

}