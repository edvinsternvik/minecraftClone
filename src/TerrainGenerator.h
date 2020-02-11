#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

class Chunk;

class TerrainGenerator {
public:
	TerrainGenerator();
	~TerrainGenerator();

	void addChunkToQueue(Chunk* chunk);
	void addChunkToDeleteQueue(Chunk* chunk);

	void generate();

private:
	void generateChunks();
	void deleteChunks();

	static void generateTerrainThreadFunction(TerrainGenerator* terrainGenerator);

private:
	std::thread m_generateTerrainThread;
	std::mutex m_mutex;
	std::condition_variable m_conditionVariable;
	std::queue<Chunk*> m_chunksToBeGenerated;
	std::queue<Chunk*> m_chunksToBeDeleted;

	bool m_ready = false;
	bool m_close = false;
};