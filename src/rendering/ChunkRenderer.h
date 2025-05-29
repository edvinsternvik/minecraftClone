#pragma once
#include "../world/Chunk.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <vector>
#include <array>
#include <memory>
#include <unordered_map>

class ChunkRenderData {
public:
    ChunkRenderData();
    ChunkRenderData(const ChunkRenderData&) = delete;
    ~ChunkRenderData();
    void generateChunkMesh(const Chunk* chunk);
    void updateChunkMesh(const Chunk* chunk);
    void update_vertex_buffer();

    inline unsigned int* getChunkSegmentData(int index) { return &chunkMesh[index][0][0]; }
    inline int getChunkSegmentMeshVerticiesCount(int index) const { return chunkMesh[index].size() * 6; }
    inline int getChunkMeshVerticiesCount() const { return chunkMeshSize; }
    bool isSolid(const int& localX, const int& y, const int& localZ, const int& chunkWorldX, const int& chunkWorldY, const Chunk* chunk);
    bool isOpaque(const int& localX, const int& y, const int& localZ, const int& chunkWorldX, const int& chunkWorldY, const Chunk* chunk);

    void bind();

private:
    void generateChunkSegmentMesh(const Chunk* chunk, int index);


private:
    std::array<std::vector<std::array<unsigned int, 6>>, CHUNK_SEGMENTS> chunkMesh;
    int chunkMeshSize;

    std::unique_ptr<VertexArray> m_vertexArray;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
};

using ChunkRenderDataMap = std::unordered_map<Vector2i, std::unique_ptr<ChunkRenderData>>;

class ChunkRenderer {
public:
    void prepare_chunks(const ChunkMap* chunks);

    ChunkRenderDataMap::iterator chunk_render_data_begin() { return m_render_data.begin(); }
    ChunkRenderDataMap::iterator chunk_render_data_end() { return m_render_data.end(); }

private:
    ChunkRenderDataMap m_render_data;
};
