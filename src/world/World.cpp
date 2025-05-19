#include "World.h"
#include "../GameObject.h"
#include "Chunk.h"

World::World(unsigned int seed)
    : m_seed(seed), m_noiseGenerator() {
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
}

void World::update(float deltaTime) {
	for(auto& go : gameObjects) {
		go->update(deltaTime);
	}
}

void World::update_chunks(int center_x, int center_z) {
    for(auto& chunk : m_chunkMap) chunk.second->pop_modifications();
	delete_distant_chunks(center_x, center_z);
	generate_chunks_around(center_x, center_z);
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
        otherChunk->block_update(CHUNK_WIDTH - 1, y, blockPosInChunk.y);
	}
	if(blockPosInChunk.x == CHUNK_WIDTH - 1) {
		Chunk* otherChunk = getChunk(x + 1, z);
        otherChunk->block_update(0, y, blockPosInChunk.y);
	}
	if(blockPosInChunk.y == 0) {
		Chunk* otherChunk = getChunk(x, z - 1);
        otherChunk->block_update(blockPosInChunk.x, y, CHUNK_WIDTH - 1);
	}
	if(blockPosInChunk.y == CHUNK_WIDTH - 1) {
		Chunk* otherChunk = getChunk(x, z + 1);
        otherChunk->block_update(blockPosInChunk.x, y, 0);
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

Chunk* World::getChunk(int worldX, int worldZ) {
    Vector2i chunkPos = getChunkPos(Vector2i(worldX, worldZ));
	
	auto search = m_chunkMap.find(chunkPos);
	if(search == m_chunkMap.end()) return nullptr;

	return search->second.get();
}

void World::generate_chunks_around(int center_x, int center_z) {
	bool chunkGenerated = false;
    Vector2i playerChunkPos = getChunkPos(Vector2i(center_x, center_z));

	for(int dist = 0; dist < renderDistance; ++dist) {
		for(int x = -dist; x <= dist; ++x) {
			for(int y = -dist; y <= dist; ++y) {
                Vector2i chunkPos = Vector2i(x, y) + playerChunkPos;
                auto search = m_chunkMap.find(chunkPos);

				if(search == m_chunkMap.end()) {
					chunkGenerated = true;

					Chunk* newChunk = createChunk(chunkPos.x, chunkPos.y);
				}
			}
		}
	}
}

void World::delete_distant_chunks(int center_x, int center_z) {
    Vector2i playerChunkPos = getChunkPos(Vector2i(center_x, center_z));
    int deletedChunks = 0;
    
    std::vector<Vector2i> to_erase;
    for(auto it = m_chunkMap.begin(); it != m_chunkMap.end(); ) {
        Vector2i chunkPos = it->first;
        Vector2i chunkPosRelative = playerChunkPos - chunkPos;

        it++;
        if(std::abs(chunkPosRelative.x) > renderDistance || std::abs(chunkPosRelative.y) > renderDistance) {
            to_erase.push_back(chunkPos);
        }
    }

    for(auto chunk_pos : to_erase) {
        m_chunkMap.erase(chunk_pos);
    }
}
