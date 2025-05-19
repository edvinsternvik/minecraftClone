#include "ChunkRenderer.h"
#include "../Application.h"
#include "../world/World.h"
#include "BlockModelData.h"
#include <set>
#include <unordered_set>

ChunkRenderData::ChunkRenderData() : chunkMeshSize(0) {
    m_vertexArray = std::make_unique<VertexArray>();
    m_vertexArray->bind();

    const int chunkVertexBufferAttributes[] = {1};
    m_vertexBuffer = std::make_unique<VertexBuffer>(chunkVertexBufferAttributes, 1);
    m_vertexBuffer->unbind();
    m_vertexArray->unbind();
}

ChunkRenderData::~ChunkRenderData() {
}

void ChunkRenderData::generateChunkMesh(const Chunk* chunk) {
    for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
        generateChunkSegmentMesh(chunk, i);
    }

    chunkMeshSize = 0;
    for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
        chunkMeshSize += getChunkSegmentMeshVerticiesCount(i);
    }
}

void ChunkRenderData::generateChunkSegmentMesh(const Chunk* chunk, int index){
    if(index < 0 || index >= CHUNK_SEGMENTS) return;

    auto* segmentMesh = &chunkMesh[index];

    segmentMesh->resize(0);
    int chunkWorldX = chunk->chunkX * CHUNK_WIDTH;
    int chunkWorldZ = chunk->chunkZ * CHUNK_WIDTH;

    World* world = Application::getInstance().getWorld();

    int yOffset = index * CHUNK_SEGMENT_HEIGHT;
    for(int x = 0; x < CHUNK_WIDTH; ++x) {
        for(int y = yOffset; y < CHUNK_SEGMENT_HEIGHT + yOffset; ++y) {
            for(int z = 0; z < CHUNK_WIDTH; ++z) {
                const Block* block = &chunk->getBlock(x, y, z);
                int worldX = x + chunkWorldX, worldZ = z + chunkWorldZ;

                if(block->getBlockType() == BlockType::Solid) {

                    if(!isSolid(x   , y    , z + 1, chunkWorldX, chunkWorldZ, chunk)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Front, Vector3(x, y, z), block->getBlockId()));
                    if(!isSolid(x   , y    , z - 1, chunkWorldX, chunkWorldZ, chunk)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Back , Vector3(x, y, z), block->getBlockId()));
                    if(!isSolid(x - 1,y    , z    , chunkWorldX, chunkWorldZ, chunk)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Left , Vector3(x, y, z), block->getBlockId()));
                    if(!isSolid(x + 1,y    , z    , chunkWorldX, chunkWorldZ, chunk)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Right, Vector3(x, y, z), block->getBlockId()));
                    if(!isSolid(x   , y + 1, z    , chunkWorldX, chunkWorldZ, chunk)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Top  , Vector3(x, y, z), block->getBlockId()));
                    if(!isSolid(x   , y - 1, z    , chunkWorldX, chunkWorldZ, chunk)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Down , Vector3(x, y, z), block->getBlockId()));
                }
                else if(block->getBlockType() == BlockType::Transparent) {
                    if(block->getBlockId() == BlockId::Tallgrass) {
                        segmentMesh->push_back(BlockModelData::getFoliageData(BlockSide::Front, Vector3(x, y, z), block->getBlockId()));
                        segmentMesh->push_back(BlockModelData::getFoliageData(BlockSide::Right, Vector3(x, y, z), block->getBlockId()));
                        segmentMesh->push_back(BlockModelData::getFoliageData(BlockSide::Back, Vector3(x, y, z), block->getBlockId()));
                        segmentMesh->push_back(BlockModelData::getFoliageData(BlockSide::Left, Vector3(x, y, z), block->getBlockId()));
                    }
                }
            }
        }
    }

    chunkMeshSize = 0;
    for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
        chunkMeshSize += getChunkSegmentMeshVerticiesCount(i);
    }
}

void ChunkRenderData::updateChunkMesh(const Chunk* chunk){
    const std::vector<Vector3i>& modifications = chunk->get_modifications();
    std::set<int> segment_updates;

    for(Vector3i pos : modifications) {
        Vector4i segPos = chunk->calculateSegmentPosFromChunkPos(pos.x, pos.y, pos.z);

        segment_updates.insert(segPos.w);
    }

    for(int segment : segment_updates) {
        generateChunkSegmentMesh(chunk, segment);
    }
}

void ChunkRenderData::update_vertex_buffer() {
    bind();
    m_vertexBuffer->setDataSize(getChunkMeshVerticiesCount());

    int offset = 0;
    for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
        int vertexCount = getChunkSegmentMeshVerticiesCount(i);
        m_vertexBuffer->setSubData(getChunkSegmentData(i), vertexCount, offset);
        offset += vertexCount;
    }
}

void ChunkRenderData::bind() {
    m_vertexArray->bind();
    m_vertexBuffer->bind();
}

bool ChunkRenderData::isSolid(const int& localX, const int& y, const int& localZ, const int& chunkWorldX, const int& chunkWorldY, const Chunk* chunk) {
    if(localX < 0 || localX > 15 || localZ < 0 || localZ > 15) {
        World* world = Application::getInstance().getWorld();
        return world->isSolid(localX + chunkWorldX, y, localZ + chunkWorldY);
    }
    return chunk->isSolid(localX, y, localZ);
}


void ChunkRenderer::prepare_chunks(const ChunkMap* chunks) {
    std::unordered_set<Vector2i> removed_chunks;
    for(auto& chunk : m_render_data) { removed_chunks.insert(chunk.first); }
    for(auto& chunk : *chunks) { removed_chunks.erase(chunk.first); }

    for(auto chunk_coords : removed_chunks) {
        m_render_data.erase(chunk_coords);
    }

    std::unordered_set<Vector2i> updated_chunks;
    for(auto& chunk_it : *chunks) {
        Vector2i chunk_coords = chunk_it.first;
	    const Chunk* chunk = chunk_it.second.get();

        if(m_render_data.count(chunk_coords) == 0) {
            m_render_data[chunk_coords] = std::make_unique<ChunkRenderData>();
            updated_chunks.insert(chunk_coords);
            updated_chunks.insert(Vector2i(chunk_coords.x + 1, chunk_coords.y + 0));
            updated_chunks.insert(Vector2i(chunk_coords.x - 1, chunk_coords.y + 0));
            updated_chunks.insert(Vector2i(chunk_coords.x + 0, chunk_coords.y + 1));
            updated_chunks.insert(Vector2i(chunk_coords.x + 0, chunk_coords.y - 1));
        }
    }

    for(auto chunk_coords : updated_chunks) {
        auto chunk_render_data = m_render_data.find(chunk_coords);
        if(chunk_render_data == m_render_data.end()) continue;
        auto chunk = chunks->find(chunk_coords);
        if(chunk == chunks->end()) continue;

        chunk_render_data->second->generateChunkMesh(chunk->second.get());
        chunk_render_data->second->update_vertex_buffer();
    }

    for(auto& chunk : *chunks) {
        if(chunk.second->get_modifications().empty()) continue;
        auto search = m_render_data.find(chunk.first);
        if(search == m_render_data.end()) continue;
        search->second->updateChunkMesh(chunk.second.get());
        search->second->update_vertex_buffer();
    }
}
