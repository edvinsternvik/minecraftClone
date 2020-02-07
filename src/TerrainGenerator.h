#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>

class Chunk;

class TerrainGenerator {
public:
	TerrainGenerator();
	~TerrainGenerator();

	void addChunkToQueue(Chunk* chunk) {
		std::lock_guard<std::mutex> guard(m_mutex);
		m_chunksToBeGenerated.push(chunk);
	}

	void generate() {
		std::lock_guard<std::mutex> guard(m_mutex);
		m_conditionVariable.notify_one(); // Unlocks generateTerrainThread
	}

private:
	static void generateTerrainThreadFunction(TerrainGenerator* terrainGenerator);

private:
	std::thread m_generateTerrainThread;
	std::mutex m_mutex;
	std::condition_variable m_conditionVariable;
	std::queue<Chunk*> m_chunksToBeGenerated;

	bool m_ready = false;
	bool m_close = false;
};