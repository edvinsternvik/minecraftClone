#pragma once
#include "Chunk.h"
#include <vector>
#include "Camera.h"

class GameObject;

class World {
public:
	World();
	~World();

	void init();
	void update();
	
	template<class T>
	T* createGameObject() {
		T* newObject = new T();
		gameObjects.push_back(newObject);
		return newObject;
	}

	const Block* const getBlock(int x, int y, int z);
	void changeBlock(int x, int y, int z, BlockId blockId);
	bool isSolid(int x, int y, int z);

	void createChunk(int x, int y);
	void deleteChunk(int x, int y);

	// bool isSolid(int x, int y, int z);

public:
	Chunk* chunks = nullptr;
	std::vector<GameObject*> gameObjects;

private:
	Chunk* getChunk(int worldX, int worldZ);
};