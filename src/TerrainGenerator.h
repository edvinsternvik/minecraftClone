#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <unordered_map>
#include <functional>
#include "Math.h"

class Chunk;

class TerrainGenerator {
public:
	TerrainGenerator(std::unordered_map<Vector2i, std::unique_ptr<Chunk>>* chunkMap);
	~TerrainGenerator();

	void addChunkToGenerateQueue(Vector2i chunkPos);
	void deleteChunk(Vector2i chunkPos);

	void generate();

private:
	static void generateTerrainThreadFunction(TerrainGenerator* terrainGenerator);

private:
	std::thread m_generateTerrainThread;
	std::mutex m_jobsMutex;
	std::mutex m_chunkMapMutex;
	std::condition_variable m_conditionVariable;
	std::queue<std::function<void()>> m_jobs;

    std::unordered_map<Vector2i, std::unique_ptr<Chunk>>* m_chunkMap;
	bool m_ready = false;
	bool m_close = false;
};
