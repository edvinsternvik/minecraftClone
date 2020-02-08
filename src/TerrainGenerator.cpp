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

void TerrainGenerator::generateTerrainThreadFunction(TerrainGenerator* terrainGenerator) {

	while (!terrainGenerator->m_close) {
		{
			std::unique_lock<std::mutex> lock(terrainGenerator->m_mutex);

			terrainGenerator->m_conditionVariable.wait(lock);
		
			lock.unlock();
		}

		while (!terrainGenerator->m_chunksToBeGenerated.empty()) {
			Chunk* chunk = terrainGenerator->m_chunksToBeGenerated.front();
			terrainGenerator->m_chunksToBeGenerated.pop();
			
			if(chunk) chunk->generateChunkMesh(); //Probably not safe if chunk gets deleted when generating chunkmesh
		}
	}
}