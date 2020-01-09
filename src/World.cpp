#include "World.h"
#include "GameObject.h"
#include "Camera.h"

World::World() : m_player(nullptr) {

}

World::~World() {
	for(GameObject* go : gameObjects) {
		delete go;
	}

	Chunk* tail = chunks;
	while(tail) {
		Chunk* next = tail->next;
		if(tail) delete tail;

		tail = next;
	}
}

void World::init() {
	for(int i = -4; i < 4; ++i) {
		for(int j = -4; j < 4; ++j) {
			createChunk(i, j);
		}
	}

	for(GameObject* go : gameObjects) {
		go->init();
	}
	
	Chunk* tail = chunks;
	while(tail) {
		tail->generateChunkMesh();
		if(!tail) break;
		tail = tail->next;
	}
}

void World::update() {
	for(GameObject* go : gameObjects) {
		go->update();
	}

	// Create chunks
	generateChunksAroundPlayer(1);
	deleteChunksNotAroundPlayer(1);
}

void World::setPlayer(GameObject* player) {
	m_player = player;
}

const Block* const World::getBlock(int x, int y, int z) {
	Chunk* chunk = getChunk(x, z);
	if(chunk == nullptr) return nullptr;

	int chunkX = x % CHUNK_WIDTH, chunkZ = z % CHUNK_WIDTH;
	if(chunkX < 0) chunkX = 16 - std::abs(chunkX);
	if(chunkZ < 0) chunkZ = 16 - std::abs(chunkZ);
	return &chunk->getBlock(x, y, z);
}

void World::changeBlock(int x, int y, int z, BlockId blockId) {
	Chunk* chunk = getChunk(x, z);
	if(chunk == nullptr) {
		return;
	}
		// std::cout << "a" << std::endl;

	int chunkX = x % CHUNK_WIDTH, chunkZ = z % CHUNK_WIDTH;
	if(chunkX < 0) chunkX = 16 - std::abs(chunkX);
	if(chunkZ < 0) chunkZ = 16 - std::abs(chunkZ);
	chunk->changeBlock(chunkX, y, chunkZ, blockId);

	if(chunkX == 0) {
		Chunk* otherChunk = getChunk(x - 1, z);
		if(otherChunk) otherChunk->generateChunkMesh();
	}
	if(chunkX == CHUNK_WIDTH - 1) {
		Chunk* otherChunk = getChunk(x + 1, z);
		if(otherChunk) otherChunk->generateChunkMesh();
	}
	if(chunkZ == 0) {
		Chunk* otherChunk = getChunk(x, z - 1);
		if(otherChunk) otherChunk->generateChunkMesh();
	}
	if(chunkZ == CHUNK_WIDTH - 1) {
		Chunk* otherChunk = getChunk(x, z + 1);
		if(otherChunk) otherChunk->generateChunkMesh();
	}
}

bool World::isSolid(int x, int y, int z) {
	Chunk* chunk = getChunk(x, z);
	if(chunk == nullptr) {
		// std::cout << x << ", " << z << " : " << std::floor(x / (float)CHUNK_WIDTH) << ", " << std::floor(z / (float)CHUNK_WIDTH) << std::endl;
		return false;
	}

	int chunkX = x % CHUNK_WIDTH, chunkZ = z % CHUNK_WIDTH;
	if(chunkX < 0) chunkX = 16 - std::abs(chunkX);
	if(chunkZ < 0) chunkZ = 16 - std::abs(chunkZ);
	return chunk->isSolid(chunkX, y, chunkZ);
} 

Chunk* World::createChunk(int x, int y) {
	Chunk* newChunk = new Chunk(x, y);

	if(chunks != nullptr) {
		newChunk->next = chunks;
	}

	chunks = newChunk;
	return newChunk;
}

void World::deleteChunk(int x, int y) {
	Chunk* before = nullptr;
	Chunk* tail = chunks;
	
	while(tail != nullptr) {
		if(tail->chunkX == x && tail->chunkZ == y) {
			if(before) before->next = tail->next;
			else chunks = tail->next;

			Chunk* temp = tail;

			tail = tail->next;

			delete temp;
		}
		else {
			before = tail;
			tail = tail->next;
		}
	}
}

Chunk* World::getChunk(int worldX, int worldZ) {
	int x = std::floor(worldX / (float)CHUNK_WIDTH), z = std::floor(worldZ / (float)CHUNK_WIDTH);
	Chunk* tail = chunks;
	while(tail) {
		if(tail->chunkX == x && tail->chunkZ == z) {
			return tail;
		}
		tail = tail->next;
	}
	
	return nullptr;
}

void World::generateChunksAroundPlayer(int maxChunksPerFrame) {
	int chunksGenerated = 0;
	int playerChunkX = std::floor(m_player->getPosition().x / (float)CHUNK_WIDTH);
	int playerChunkZ = std::floor(m_player->getPosition().z / (float)CHUNK_WIDTH); 
	for(int dist = 0; dist < renderDistance; ++dist) {
		for(int x = -dist; x <= dist; ++x) {
			for(int y = -dist; y <= dist; ++y) {
				int chunkX = x + playerChunkX, chunkZ = y + playerChunkZ;
				bool chunkExists = false;
				Chunk* tail = chunks;
				while(tail != nullptr) {
					if(tail->chunkX == chunkX && tail->chunkZ == chunkZ) {
						chunkExists = true;
						break;
					}

					tail = tail->next;
				}

				if(!chunkExists) {
					Chunk* newChunk = createChunk(chunkX, chunkZ);
					newChunk->generateChunkMesh();
					if(++chunksGenerated >= maxChunksPerFrame) return;
				}
			}
		}
	}
}

void World::deleteChunksNotAroundPlayer(int maxChunksPerFrame) {
	int playerChunkX = std::floor(m_player->getPosition().x / (float)CHUNK_WIDTH);
	int playerChunkZ = std::floor(m_player->getPosition().z / (float)CHUNK_WIDTH); 
	int deletedChunks = 0;
	
	Chunk* tail = chunks;
	while(tail != nullptr) {
		int chunkX = playerChunkX - tail->chunkX, chunkZ = playerChunkZ - tail->chunkZ;
		if(std::abs(chunkX) > renderDistance || std::abs(chunkZ) > renderDistance) {
			deleteChunk(tail->chunkX, tail->chunkZ);
			if(++deletedChunks >= maxChunksPerFrame) return;
		}
		tail = tail->next;
	}
}