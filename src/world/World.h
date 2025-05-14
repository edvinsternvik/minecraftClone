#pragma once
#include "Chunk.h"
#include <vector>
#include <unordered_map>
#include "../misc/PerlinNoise.h"
#include <memory>

#include "../Camera.h"

class GameObject;

class World {
public:

public:
	World(unsigned int seed);
	~World();

	void init();
	void update(float deltaTime);
    void update_chunks(int center_x, int center_z);
	
	template<class T>
	T* createGameObject() {
		gameObjects.push_back(std::make_unique<T>());
		return dynamic_cast<T*>(gameObjects.back().get());
	}

	const Block* const getBlock(int x, int y, int z);
	const unsigned int& getSeed() const { return m_seed; }
	void changeBlock(int x, int y, int z, BlockId blockId);
	bool isSolid(int x, int y, int z);
    Vector2i getWorldPos(Vector2i chunkPos);
    Vector2i getChunkPos(Vector2i worldPos);
    Vector2i getBlockPosInChunk(Vector2i worldPos);

	Chunk* createChunk(int x, int y);

    const ChunkMap* get_chunk_map() const { return &m_chunkMap; }
    ChunkMap::iterator chunkMapBegin() {
        return m_chunkMap.begin();
    }

    ChunkMap::iterator chunkMapEnd() {
        return m_chunkMap.end();
    }

public:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	int renderDistance = 12;

private:
	Chunk* getChunk(int worldX, int worldZ);
	void generate_chunks_around(int center_x, int center_z);
	void delete_distant_chunks(int center_x, int center_z);

private:
	ChunkMap m_chunkMap;
	const unsigned int m_seed;
	PerlinNoise m_noiseGenerator;
};
