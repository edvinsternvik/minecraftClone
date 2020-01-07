#pragma once
#include "Block.h"
#include <vector>
#include <array>
#include "Math.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 64

enum CubeSide {
	Front, Back, Left, Right, Top, Down
};

class CubeModelData {
public:
	static std::array<float, 48> getData(CubeSide cubeSide, Vector3 position, float textureX, float textureY) {
		if(cubeSide == CubeSide::Front) {
			return {
				0.0f  + position.x, 0.0f  + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 0.0f + textureX, 0.0f + textureY,
				 1.0f + position.x, 0.0f  + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 1.0f + textureX, 0.0f + textureY,
				 1.0f + position.x,  1.0f + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 1.0f + textureX, 1.0f + textureY,

				 1.0f + position.x,  1.0f + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 1.0f + textureX, 1.0f + textureY,
				0.0f  + position.x,  1.0f + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 0.0f + textureX, 1.0f + textureY,
				0.0f  + position.x, 0.0f  + position.y, 1.0f + position.z, 0.0, 0.0, 1.0, 0.0f + textureX, 0.0f + textureY
			};
		}
		else if(cubeSide == CubeSide::Back) {
			return {
				 1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 1.0f + textureX, 1.0f + textureY,
				 1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 1.0f + textureX, 0.0f + textureY,
				0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 0.0f + textureX, 0.0f + textureY,

				0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 0.0f + textureX, 0.0f + textureY,
				0.0f  + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 0.0f + textureX, 1.0f + textureY,
				 1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 0.0, -1.0, 1.0f + textureX, 1.0f + textureY
			};
		}
		else if(cubeSide == CubeSide::Left) {
			return {
				0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, -1.0, 0.0, 0.0, 0.0f + textureX, 0.0f + textureY,
				0.0f  + position.x, 0.0f  + position.y,  1.0f + position.z, -1.0, 0.0, 0.0, 1.0f + textureX, 0.0f + textureY,
				0.0f  + position.x,  1.0f + position.y,  1.0f + position.z, -1.0, 0.0, 0.0, 1.0f + textureX, 1.0f + textureY,

				0.0f  + position.x,  1.0f + position.y,  1.0f + position.z, -1.0, 0.0, 0.0, 1.0f + textureX, 1.0f + textureY,
				0.0f  + position.x,  1.0f + position.y, 0.0f  + position.z, -1.0, 0.0, 0.0, 0.0f + textureX, 1.0f + textureY,
				0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, -1.0, 0.0, 0.0, 0.0f + textureX, 0.0f + textureY
			};
		}
		else if(cubeSide == CubeSide::Right) {
			return {
				1.0f + position.x,  1.0f + position.y,  1.0f + position.z, 1.0, 0.0, 0.0, 1.0f + textureX, 1.0f + textureY,
				1.0f + position.x, 0.0f  + position.y,  1.0f + position.z, 1.0, 0.0, 0.0, 1.0f + textureX, 0.0f + textureY,
				1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 1.0, 0.0, 0.0, 0.0f + textureX, 0.0f + textureY,

				1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 1.0, 0.0, 0.0, 0.0f + textureX, 0.0f + textureY,
				1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 1.0, 0.0, 0.0, 0.0f + textureX, 1.0f + textureY,
				1.0f + position.x,  1.0f + position.y,  1.0f + position.z, 1.0, 0.0, 0.0, 1.0f + textureX, 1.0f + textureY
			};
		}
		else if(cubeSide == CubeSide::Top) {
			return {
				0.0f  + position.x,  1.0f + position.y,  1.0f + position.z, 0.0, 1.0, 0.0, 0.0f + textureX, 0.0f + textureY,
				 1.0f + position.x,  1.0f + position.y,  1.0f + position.z, 0.0, 1.0, 0.0, 1.0f + textureX, 0.0f + textureY,
				 1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 1.0, 0.0, 1.0f + textureX, 1.0f + textureY,

				 1.0f + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 1.0, 0.0, 1.0f + textureX, 1.0f + textureY,
				0.0f  + position.x,  1.0f + position.y, 0.0f  + position.z, 0.0, 1.0, 0.0, 0.0f + textureX, 1.0f + textureY,
				0.0f  + position.x,  1.0f + position.y,  1.0f + position.z, 0.0, 1.0, 0.0, 0.0f + textureX, 0.0f + textureY
			};
		}
		else {
			return {
				 1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, -1.0, 0.0, 1.0f + textureX, 1.0f + textureY,
				 1.0f + position.x, 0.0f  + position.y,  1.0f + position.z, 0.0, -1.0, 0.0, 1.0f + textureX, 0.0f + textureY,
				0.0f  + position.x, 0.0f  + position.y,  1.0f + position.z, 0.0, -1.0, 0.0, 0.0f + textureX, 0.0f + textureY,

				0.0f  + position.x, 0.0f  + position.y,  1.0f + position.z, 0.0, -1.0, 0.0, 0.0f + textureX, 0.0f + textureY,
				0.0f  + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, -1.0, 0.0, 0.0f + textureX, 1.0f + textureY,
				 1.0f + position.x, 0.0f  + position.y, 0.0f  + position.z, 0.0, -1.0, 0.0, 1.0f + textureX, 1.0f + textureY
			};
		}
	}
};

class Chunk {
public:
	Chunk(int chunkX, int chunkZ);
	~Chunk();

	bool isSolid(int x, int y, int z);
	inline const Block& getBlock(int x, int y, int z) {
		return BlockManager::getBlock(blocks[x][y][z]);
	}
	void changeBlock(int x, int y, int z, BlockId blockId);

	inline float* getChunkMeshData() { return &chunkMesh[0][0]; }
	inline int getChunkMeshVerticiesCount() const { return chunkMesh.size() * 6; }

private:
	void generateChunkMesh();

public:
	const int chunkX, chunkZ;
	BlockId blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];
	std::vector<std::array<float, 48>> chunkMesh;
	Chunk* next = nullptr;

	friend class World;

};