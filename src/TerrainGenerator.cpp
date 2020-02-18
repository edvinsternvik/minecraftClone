#include "TerrainGenerator.h"
#include "ChunkRenderer.h"

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

void TerrainGenerator::generateChunks(std::queue<Chunk*>& chunksToBeGenerated) {
	while (!chunksToBeGenerated.empty()) {
		Chunk* chunk = chunksToBeGenerated.front();
		chunksToBeGenerated.pop();
		
		if(chunk) chunk->getChunkRenderer()->generateChunkMesh();
	}
}

void TerrainGenerator::deleteChunks(std::queue<Chunk*>& chunksToBeDeleted) {
	while (!chunksToBeDeleted.empty()) {
		Chunk* chunk = chunksToBeDeleted.front();
		chunksToBeDeleted.pop();
		
		if(chunk) delete chunk;
	}
}

void TerrainGenerator::generateTerrainThreadFunction(TerrainGenerator* terrainGenerator) {

	while (!terrainGenerator->m_close) {
		std::queue<Chunk*> localChunksToBeGenerated, localChunksToBeDeleted;

		{
			std::unique_lock<std::mutex> lock(terrainGenerator->m_mutex);

			terrainGenerator->m_conditionVariable.wait(lock);

			while (!terrainGenerator->m_chunksToBeGenerated.empty()) {
				Chunk* chunk = terrainGenerator->m_chunksToBeGenerated.front();
				localChunksToBeGenerated.push(chunk);

				terrainGenerator->m_chunksToBeGenerated.pop();
			}

			while (!terrainGenerator->m_chunksToBeDeleted.empty()) {
				localChunksToBeDeleted.push(terrainGenerator->m_chunksToBeDeleted.front());
				terrainGenerator->m_chunksToBeDeleted.pop();
			}
		
			lock.unlock();
		}

		terrainGenerator->generateChunks(localChunksToBeGenerated);
		terrainGenerator->deleteChunks(localChunksToBeDeleted);
	}
}