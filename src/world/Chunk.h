#pragma once
#include "Block.h"
#include "../misc/Math.h"
#include "../misc/PerlinNoise.h"
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 128
#define CHUNK_SEGMENT_HEIGHT CHUNK_WIDTH
#define CHUNK_SEGMENTS CHUNK_HEIGHT / CHUNK_SEGMENT_HEIGHT

using ChunkID = Vector2i;

class Chunk {
public:
    Chunk(int chunkX, int chunkZ, const PerlinNoise& noiseGenerator);
    Chunk(const Chunk&) = delete;
    ~Chunk();
    Chunk&  operator=(const Chunk&) = delete;

    bool isSolid(int x, int y, int z) const;
    std::optional<Block> get_block(int x, int y, int z) const;
    inline const Block& getBlock(int x, int y, int z) const {
        Vector4i segPos = calculateSegmentPosFromChunkPos(x, y, z);
        return BlockManager::getBlock(blocks[segPos.w][segPos.x][segPos.y][segPos.z]);
    }
    void changeBlock(int x, int y, int z, BlockId blockId);
    void block_update(int x, int y, int z);
    Vector4i calculateSegmentPosFromChunkPos(int x, int y, int z) const;
    const std::vector<Vector3i>& get_modifications() const;
    void pop_modifications();

public:
    const int chunkX, chunkZ;

private:
    BlockId blocks[CHUNK_SEGMENTS][CHUNK_WIDTH][CHUNK_SEGMENT_HEIGHT][CHUNK_WIDTH];
    std::vector<Vector3i> m_modifications;

    friend class World;
};

using ChunkMap = std::unordered_map<Vector2i, std::unique_ptr<Chunk>>;
