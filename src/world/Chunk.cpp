#include "Chunk.h"
#include "Block.h"

#define CHUNK_WIDTH_INVERSE 1.0 / (float)CHUNK_WIDTH
#define NOISE_OFFSET 10000.0

Chunk::Chunk(int chunkX, int chunkZ, const PerlinNoise& noiseGenerator) : chunkX(chunkX), chunkZ(chunkZ) {
    for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
        int chunkSegmentYPos = i * CHUNK_SEGMENT_HEIGHT;

        for(int x = 0; x < CHUNK_WIDTH; ++x) {
            for(int z = 0; z < CHUNK_WIDTH; ++z) {
                for(int y = 0; y < CHUNK_SEGMENT_HEIGHT; ++y) {
                    int yPos = y + chunkSegmentYPos;

                    BlockId blockId = BlockId::Air;

                    float noiseX = ((float)x * CHUNK_WIDTH_INVERSE + (float)chunkX) + NOISE_OFFSET;
                    float noiseZ = ((float)z * CHUNK_WIDTH_INVERSE + (float)chunkZ) + NOISE_OFFSET;

                    float noiseValue = 32;
                    noiseValue += noiseGenerator.noise(noiseX * 1.0 + NOISE_OFFSET, noiseZ * 1.0 + NOISE_OFFSET) * 4;
                    noiseValue += noiseGenerator.noise(noiseX * 0.15, noiseZ * 0.15) * 24;

                    if(yPos < noiseValue) blockId = BlockId::Stone;
                    else if(yPos < noiseValue + 3) blockId = BlockId::Dirt;
                    else if(yPos < noiseValue + 4) blockId = BlockId::Grass;
                    else if(yPos < noiseValue + 5 && rand() % 10 == 0 ) blockId = BlockId::Tallgrass;

                    blocks[i][x][y][z] = blockId;
                }
            }
        }
    }
}

Chunk::~Chunk() {
}

void Chunk::changeBlock(int x, int y, int z, BlockId blockId) {
    if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
        return;
    }

    Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
    if(y >= 0 && y < CHUNK_HEIGHT && blocks[segPos.w][segPos.x][segPos.y][segPos.z] != blockId) {
        blocks[segPos.w][segPos.x][segPos.y][segPos.z] = blockId;
        block_update(x, y, z);
        block_update(x+1, y, z);
        block_update(x-1, y, z);
        block_update(x, y+1, z);
        block_update(x, y-1, z);
        block_update(x, y, z+1);
        block_update(x, y, z-1);
    }

}

void Chunk::block_update(int x, int y, int z) {
    if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
        return;
    }

    auto block = get_block(x, y, z);
    auto block_below = get_block(x, y - 1, z);
    if(block.has_value() && block->getBlockId() == BlockId::Tallgrass
        && block_below.has_value() && block_below->getBlockType() == BlockType::Transparent) {
        changeBlock(x, y, z, BlockId::Air);
    }

    m_modifications.push_back(Vector3i(x, y, z));
}

bool Chunk::isSolid(int x, int y, int z) const {
    if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
        return false;
    }

    return get_block(x, y, z)->getBlockType() == BlockType::Solid;
}

std::optional<Block> Chunk::get_block(int x, int y, int z) const {
    if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
        return std::optional<Block>();
    }
    Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
    return BlockManager::getBlock(blocks[segPos.w][segPos.x][segPos.y][segPos.z]);
}

Vector4i Chunk::calculateSegmentPosFromChunkPos(int x, int y, int z) const {
    return Vector4i(x, y % CHUNK_SEGMENT_HEIGHT, z, y >> 4);
}

const std::vector<Vector3i>& Chunk::get_modifications() const {
    return m_modifications;
}

void Chunk::pop_modifications() {
    m_modifications.clear();
}
