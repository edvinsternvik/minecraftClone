#include "World.h"
#include "GameObject.h"
#include "Camera.h"
#include "ChunkRenderer.h"

World::World(unsigned int seed)
    : m_player(nullptr), m_seed(seed), m_noiseGenerator(), m_terrainGenerator(&m_chunkMap) {
	m_noiseGenerator.seed(seed);
}

World::~World() {
}

void World::init() {
	for(int i = -renderDistance; i < renderDistance; ++i) {
		for(int j = -renderDistance; j < renderDistance; ++j) {
			createChunk(i, j);
		}
	}

	for(auto& go : gameObjects) {
		go->init();
	}

    for(auto& chunkPair : m_chunkMap) {
        chunkPair.second->getChunkRenderer()->generateChunkMesh();
    }
}

void World::update(float deltaTime) {
	for(auto& go : gameObjects) {
		go->update(deltaTime);
	}

	// Create chunks
	deleteChunksNotAroundPlayer();
	generateChunksAroundPlayer();
}

void World::setPlayer(GameObject* player) {
	m_player = player;
}

const Block* const World::getBlock(int x, int y, int z) {
	Chunk* chunk = getChunk(x, z);
	if(chunk == nullptr) return nullptr;

	return &chunk->getBlock(x, y, z);
}

void World::changeBlock(int x, int y, int z, BlockId blockId) {
	Chunk* chunk = getChunk(x, z);
	if(chunk == nullptr) {
		return;
	}

    Vector2i blockPosInChunk = getBlockPosInChunk(Vector2i(x, z));
	chunk->changeBlock(blockPosInChunk.x, y, blockPosInChunk.y, blockId);

	if(blockPosInChunk.x == 0) {
		Chunk* otherChunk = getChunk(x - 1, z);
		if(otherChunk) otherChunk->getChunkRenderer()->updateChunkMesh(x, y, z);
	}
	if(blockPosInChunk.x == CHUNK_WIDTH - 1) {
		Chunk* otherChunk = getChunk(x + 1, z);
		if(otherChunk) otherChunk->getChunkRenderer()->updateChunkMesh(x, y, z);
	}
	if(blockPosInChunk.y == 0) {
		Chunk* otherChunk = getChunk(x, z - 1);
		if(otherChunk) otherChunk->getChunkRenderer()->updateChunkMesh(x, y, z);
	}
	if(blockPosInChunk.y == CHUNK_WIDTH - 1) {
		Chunk* otherChunk = getChunk(x, z + 1);
		if(otherChunk) otherChunk->getChunkRenderer()->updateChunkMesh(x, y, z);
	}
}

bool World::isSolid(int x, int y, int z) {
	Chunk* chunk = getChunk(x, z);
	if(chunk == nullptr) {
		return true;
	}

    Vector2i blockPosInChunk = getBlockPosInChunk(Vector2i(x, z));
	return chunk->isSolid(blockPosInChunk.x, y, blockPosInChunk.y);
} 

Vector2i World::getWorldPos(Vector2i chunkPos) {
    static_assert(CHUNK_WIDTH == 16, "World::getWorldPos() assumes that CHUNK_WIDTH == 16");
    return Vector2i(chunkPos.x << 4, chunkPos.y << 4);
}

Vector2i World::getChunkPos(Vector2i worldPos) {
    static_assert(CHUNK_WIDTH == 16, "World::getChunkPos() assumes that CHUNK_WIDTH == 16");
    return Vector2i(worldPos.x >> 4, worldPos.y >> 4);
}

Vector2i World::getBlockPosInChunk(Vector2i worldPos) {
    Vector2i res;
	res.x = worldPos.x % CHUNK_WIDTH;
    res.y = worldPos.y % CHUNK_WIDTH;
	if(res.x < 0) res.x = CHUNK_WIDTH - std::abs(res.x);
	if(res.y < 0) res.y = CHUNK_WIDTH - std::abs(res.y);
    return res;
}

Chunk* World::createChunk(int x, int y) {
	auto r = m_chunkMap.insert(std::pair<Vector2i, std::unique_ptr<Chunk>>(Vector2i(x, y), std::make_unique<Chunk>(x, y, m_noiseGenerator)));

	return r.first->second.get();
}

void World::deleteChunk(int x, int y) {
    m_terrainGenerator.deleteChunk(Vector2i(x, y));
}

Chunk* World::getChunk(const int& worldX, const int& worldZ) {
    Vector2i chunkPos = getChunkPos(Vector2i(worldX, worldZ));
	
	auto search = m_chunkMap.find(chunkPos);
	if(search == m_chunkMap.end()) return nullptr;

	return search->second.get();
}

void World::generateChunksAroundPlayer() {
	bool chunkGenerated = false;
    Vector2i playerChunkPos = getChunkPos(Vector2i(m_player->getPosition().x, m_player->getPosition().z));

	for(int dist = 0; dist < renderDistance; ++dist) {
		for(int x = -dist; x <= dist; ++x) {
			for(int y = -dist; y <= dist; ++y) {
                Vector2i chunkPos = Vector2i(x, y) + playerChunkPos;
                auto search = m_chunkMap.find(chunkPos);

				if(search == m_chunkMap.end()) {
					chunkGenerated = true;

					Chunk* newChunk = createChunk(chunkPos.x, chunkPos.y);
                    m_terrainGenerator.addChunkToGenerateQueue(chunkPos);

                    Vector2i chunkWorldPos = getWorldPos(chunkPos);

					Chunk* chunk1 = getChunk(chunkWorldPos.x - CHUNK_WIDTH, chunkWorldPos.y);
					Chunk* chunk2 = getChunk(chunkWorldPos.x, chunkWorldPos.y - CHUNK_WIDTH);
					Chunk* chunk3 = getChunk(chunkWorldPos.x + CHUNK_WIDTH, chunkWorldPos.y);
					Chunk* chunk4 = getChunk(chunkWorldPos.x, chunkWorldPos.y + CHUNK_WIDTH);

					if(chunk1 != nullptr) m_terrainGenerator.addChunkToGenerateQueue(Vector2i(chunk1->chunkX, chunk1->chunkZ));
					if(chunk2 != nullptr) m_terrainGenerator.addChunkToGenerateQueue(Vector2i(chunk2->chunkX, chunk2->chunkZ));
					if(chunk3 != nullptr) m_terrainGenerator.addChunkToGenerateQueue(Vector2i(chunk3->chunkX, chunk3->chunkZ));
					if(chunk4 != nullptr) m_terrainGenerator.addChunkToGenerateQueue(Vector2i(chunk4->chunkX, chunk4->chunkZ));
				}
			}
		}

		if (chunkGenerated) {
			m_terrainGenerator.generate();
		}
	}
}

void World::deleteChunksNotAroundPlayer() {
    Vector2i playerChunkPos = getChunkPos(Vector2i(m_player->getPosition().x, m_player->getPosition().z));
    int deletedChunks = 0;
    
    for(auto it = m_chunkMap.begin(); it != m_chunkMap.end(); ) {
        Vector2i chunkPos = it->first;
        Vector2i chunkPosRelative = playerChunkPos - chunkPos;

        it++;
        if(std::abs(chunkPosRelative.x) > renderDistance || std::abs(chunkPosRelative.y) > renderDistance) {
            deleteChunk(chunkPos.x, chunkPos.y);
        }
    }
}
