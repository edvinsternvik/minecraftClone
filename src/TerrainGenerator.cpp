#include "TerrainGenerator.h"
#include "ChunkRenderer.h"

void generateChunkJobFunction(std::unordered_map<Vector2i, std::unique_ptr<Chunk>>* chunkMap, Vector2i chunkPos) {
    auto search = chunkMap->find(chunkPos);
    if(search != chunkMap->end()) {
        search->second->getChunkRenderer()->generateChunkMesh();
    }
}

TerrainGenerator::TerrainGenerator(std::unordered_map<Vector2i, std::unique_ptr<Chunk>>* chunkMap)
    : m_generateTerrainThread(&TerrainGenerator::generateTerrainThreadFunction, this), m_chunkMap(chunkMap) {
}

TerrainGenerator::~TerrainGenerator() {
    m_close = true;
    m_conditionVariable.notify_one();
	m_generateTerrainThread.join();
}

void TerrainGenerator::addChunkToGenerateQueue(Vector2i chunkPos) {
	std::lock_guard<std::mutex> guard(m_jobsMutex);

    m_jobs.push([=](){
        generateChunkJobFunction(m_chunkMap, chunkPos);
    });
}

void TerrainGenerator::deleteChunk(Vector2i chunkPos) {
	std::lock_guard<std::mutex> guard(m_chunkMapMutex);

    auto search = m_chunkMap->find(chunkPos);
    if(search != m_chunkMap->end()) {
        m_chunkMap->erase(search);
    }
}

void TerrainGenerator::generate() {
	std::lock_guard<std::mutex> guard(m_jobsMutex);
	m_conditionVariable.notify_one(); // Unlocks generateTerrainThread
}

void TerrainGenerator::generateTerrainThreadFunction(TerrainGenerator* terrainGenerator) {

	while (!terrainGenerator->m_close) {
		std::queue<Chunk*> localChunksToBeGenerated, localChunksToBeDeleted;

        std::function<void()> job;
		{
			std::unique_lock<std::mutex> lock(terrainGenerator->m_jobsMutex);

			terrainGenerator->m_conditionVariable.wait(lock, [terrainGenerator]() {
                return !terrainGenerator->m_jobs.empty() || terrainGenerator->m_close;
            });

            if(!terrainGenerator->m_jobs.empty()) {
                job = terrainGenerator->m_jobs.front();
                terrainGenerator->m_jobs.pop();
            }

			lock.unlock();
		}

        if(job) {
            std::lock_guard<std::mutex> guard(terrainGenerator->m_chunkMapMutex);
            job();
        }
	}
}
