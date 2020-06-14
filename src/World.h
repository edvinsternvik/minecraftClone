#pragma once
#include "Chunk.h"
#include <vector>
#include <unordered_map>
#include "Camera.h"
#include "TerrainGenerator.h"

class GameObject;

class World {
public:
	World();
	~World();

	void init();
	void update(float deltaTime);
	
	template<class T>
	T* createGameObject() {
		T* newObject = new T();
		gameObjects.push_back(newObject);
		return newObject;
	}

	const Block* const getBlock(int x, int y, int z);
	void setPlayer(GameObject* player);
	void changeBlock(int x, int y, int z, BlockId blockId);
	bool isSolid(int x, int y, int z);

	Chunk* createChunk(int x, int y);
	void deleteChunk(int x, int y);

	// bool isSolid(int x, int y, int z);

public:
	Chunk* chunks = nullptr;
	std::vector<GameObject*> gameObjects;
	int renderDistance = 6;

private:
	Chunk* getChunk(const int& worldX, const int& worldZ);
	void generateChunksAroundPlayer();
	void deleteChunksNotAroundPlayer(int maxChunksPerFrame);
	inline long long int getChunkMapKey(const int& worldX, const int& worldZ) { return ((long long int)worldX) << 32 | (unsigned int)worldZ; }

private:
	GameObject* m_player;
	TerrainGenerator m_terrainGenerator;
	std::unordered_map<long long int, Chunk*> m_chunkMap;
};