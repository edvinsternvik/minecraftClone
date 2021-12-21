#pragma once
#include "Chunk.h"
#include <vector>
#include <unordered_map>
#include "Camera.h"
#include "TerrainGenerator.h"
#include "PerlinNoise.h"
#include <memory>

class GameObject;

class World {
public:
    using ChunkMapType = std::unordered_map<Vector2i, std::unique_ptr<Chunk>>;

public:
	World(unsigned int seed);
	~World();

	void init();
	void update(float deltaTime);
	
	template<class T>
	T* createGameObject() {
		gameObjects.push_back(std::make_unique<T>());
		return dynamic_cast<T*>(gameObjects.back().get());
	}

	const Block* const getBlock(int x, int y, int z);
	const unsigned int& getSeed() const { return m_seed; }
	void setPlayer(GameObject* player);
	void changeBlock(int x, int y, int z, BlockId blockId);
	bool isSolid(int x, int y, int z);
    Vector2i getWorldPos(Vector2i chunkPos);
    Vector2i getChunkPos(Vector2i worldPos);
    Vector2i getBlockPosInChunk(Vector2i worldPos);

	Chunk* createChunk(int x, int y);
	void deleteChunk(int x, int y);

    ChunkMapType::iterator chunkMapBegin() {
        return m_chunkMap.begin();
    }

    ChunkMapType::iterator chunkMapEnd() {
        return m_chunkMap.end();
    }

public:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	int renderDistance = 12;

private:
	Chunk* getChunk(const int& worldX, const int& worldZ);
	void generateChunksAroundPlayer();
	void deleteChunksNotAroundPlayer();

private:
	GameObject* m_player;
	TerrainGenerator m_terrainGenerator;
	ChunkMapType m_chunkMap;
	const unsigned int m_seed;
	PerlinNoise m_noiseGenerator;
};
