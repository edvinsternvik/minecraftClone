#include "TerrainGenerator.h"
#include "Chunk.h"

TerrainGenerator::TerrainGenerator() : m_generateTerrainThread(&TerrainGenerator::generateTerrainThreadFunction, this) {

	//std::lock_guard<std::mutex> lk(m_mutex);
	//m_conditionVariable.notify_one();
}

TerrainGenerator::~TerrainGenerator() {
	m_close = true;
	m_conditionVariable.notify_one();
	m_generateTerrainThread.join();
}

void TerrainGenerator::addChunkToQueue(Chunk* chunk) {
	std::lock_guard<std::mutex> guard(m_mutex);
	m_chunksToBeGenerated.push(chunk);
}

void TerrainGenerator::addChunkToDeleteQueue(Chunk* chunk) {
	std::lock_guard<std::mutex> guard(m_mutex);
	m_chunksToBeDeleted.push(chunk);
}

void TerrainGenerator::generate() {
	std::lock_guard<std::mutex> guard(m_mutex);
	m_conditionVariable.notify_one(); // Unlocks generateTerrainThread
}

void TerrainGenerator::generateChunks() {
	while (!m_chunksToBeGenerated.empty()) {
		Chunk* chunk = m_chunksToBeGenerated.front();
		m_chunksToBeGenerated.pop();
		
		if(chunk) chunk->generateChunkMesh();
	}
}

void TerrainGenerator::deleteChunks() {
	while (!m_chunksToBeDeleted.empty()) {
		Chunk* chunk = m_chunksToBeDeleted.front();
		m_chunksToBeDeleted.pop();
		
		if(chunk) delete chunk;
	}
}

void TerrainGenerator::generateTerrainThreadFunction(TerrainGenerator* terrainGenerator) {

	while (!terrainGenerator->m_close) {
		{
			std::unique_lock<std::mutex> lock(terrainGenerator->m_mutex);

			terrainGenerator->m_conditionVariable.wait(lock);
		
			lock.unlock();
		}

		terrainGenerator->generateChunks();
		terrainGenerator->deleteChunks();
	}
}