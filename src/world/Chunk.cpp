#include "Chunk.h"
#include "Block.h"
#include "Structures.h"
#include <cstdint>

#define CHUNK_WIDTH_INVERSE 1.0 / (float)CHUNK_WIDTH
#define NOISE_OFFSET 10000.0
#define GRASS_HASH_OFFSET 0
#define TREE_HASH_OFFSET 1

uint32_t hash( uint32_t a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

uint32_t hash_2d(uint32_t x, uint32_t y, uint32_t seed) {
    return hash(hash(hash(x) + y) + seed);
}

int calc_ground_height(int x, int z, const PerlinNoise& noise_generator) {
    Vector2i delta = { x % CHUNK_WIDTH, z % CHUNK_WIDTH };
    Vector2i chunk = { x / CHUNK_WIDTH, z / CHUNK_WIDTH };
    float noise_x = (
        (float)delta.x * CHUNK_WIDTH_INVERSE + (float)chunk.x) + NOISE_OFFSET;
    float noise_z =
        ((float)delta.y * CHUNK_WIDTH_INVERSE + (float)chunk.y) + NOISE_OFFSET;

    float noise_value = 32;
    noise_value += noise_generator.noise(noise_x * 1.0 + NOISE_OFFSET, noise_z * 1.0 + NOISE_OFFSET) * 4;
    noise_value += noise_generator.noise(noise_x * 0.15, noise_z * 0.15) * 24;
    return noise_value;
}

Chunk::Chunk(int chunkX, int chunkZ, const PerlinNoise& noise_generator) : chunkX(chunkX), chunkZ(chunkZ) {
    for(int x = 0; x < CHUNK_WIDTH; ++x) {
        for(int z = 0; z < CHUNK_WIDTH; ++z) {
            int ground_height = calc_ground_height(
                chunkX * CHUNK_WIDTH + x,
                chunkZ * CHUNK_WIDTH + z,
                noise_generator
            );

            for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
                int chunkSegmentYPos = i * CHUNK_SEGMENT_HEIGHT;

                for(int y = 0; y < CHUNK_SEGMENT_HEIGHT; ++y) {
                    int yPos = y + chunkSegmentYPos;

                    BlockId blockId = BlockId::Air;
                    if(yPos <= ground_height - 4) blockId = BlockId::Stone;
                    else if(yPos <= ground_height - 1) blockId = BlockId::Dirt;
                    else if(yPos <= ground_height - 0) blockId = BlockId::Grass;

                    blocks[i][x][y][z] = blockId;
                }
            }
        }
    }

    for(int x = -2; x < CHUNK_WIDTH + 2; ++x) {
        for(int z = -2; z < CHUNK_WIDTH + 2; ++z) {
            uint32_t tree_hash = hash_2d(
                CHUNK_WIDTH * chunkX + x,
                CHUNK_WIDTH * chunkZ + z,
                TREE_HASH_OFFSET
            );
            if(tree_hash % 128 != 0) continue;

            int y = calc_ground_height(
                chunkX * CHUNK_WIDTH + x,
                chunkZ * CHUNK_WIDTH + z,
                noise_generator
            ) + 1;

            for(int i = 0; i < std::size(Structures::tree_structure); ++i) {
                StructureBlock struct_block = Structures::tree_structure[i];
                set_block_no_override(
                    x + struct_block.pos.x,
                    y + struct_block.pos.y,
                    z + struct_block.pos.z,
                    struct_block.block
                );
            }
        }
    }

    for(int x = 0; x < CHUNK_WIDTH; ++x) {
        for(int z = 0; z < CHUNK_WIDTH; ++z) {
            uint32_t grass_hash = hash_2d(
                CHUNK_WIDTH * chunkX + x,
                CHUNK_WIDTH * chunkZ + z,
                GRASS_HASH_OFFSET
            );
            if(grass_hash % 10 != 0) continue;
            int ground_height = calc_ground_height(
                chunkX * CHUNK_WIDTH + x,
                chunkZ * CHUNK_WIDTH + z,
                noise_generator
            );

            set_block_no_override(x, ground_height + 1, z, BlockId::Tallgrass);
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
    return Vector4i(x, y % CHUNK_SEGMENT_HEIGHT, z, y / CHUNK_SEGMENT_HEIGHT);
}

const std::vector<Vector3i>& Chunk::get_modifications() const {
    return m_modifications;
}

void Chunk::pop_modifications() {
    m_modifications.clear();
}

void Chunk::set_block(int x, int y, int z, BlockId block) {
    if(x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_WIDTH) {
        return;
    }
    Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
    blocks[segPos.w][segPos.x][segPos.y][segPos.z] = block;
}

void Chunk::set_block_no_override(int x, int y, int z, BlockId block) {
    auto prev_block = get_block(x, y, z);
    if(prev_block.has_value() && prev_block->getBlockId() != BlockId::Air) return;
    set_block(x, y, z, block);
}
