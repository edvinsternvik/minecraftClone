#include "ChunkRenderer.h"
#include "../Application.h"
#include "../world/World.h"
#include "BlockModelData.h"

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

void ChunkRenderData::generateChunkMesh(Chunk* chunk) {
    for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
        generateChunkSegmentMesh(chunk, i);
    }

    chunkMeshSize = 0;
    for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
        chunkMeshSize += getChunkSegmentMeshVerticiesCount(i);
    }
}

void ChunkRenderData::generateChunkSegmentMesh(Chunk* chunk, int index){
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

    updatedChunkMesh = true;
}

void ChunkRenderData::updateChunkMesh(Chunk* chunk, int x, int y, int z){
    Vector4i segPos = chunk->calculateSegmentPosFromChunkPos(x, y, z);
    generateChunkSegmentMesh(chunk, segPos.w);

    int chunkSegmentY = y % CHUNK_SEGMENT_HEIGHT;
    if(chunkSegmentY == 0 && segPos.w > 0) generateChunkSegmentMesh(chunk, segPos.w - 1);
    if(chunkSegmentY == CHUNK_SEGMENT_HEIGHT - 1 && segPos.w < CHUNK_SEGMENTS - 1) generateChunkSegmentMesh(chunk, segPos.w + 1);
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


void ChunkRenderer::prepare_chunks(ChunkMap::iterator chunk_begin, ChunkMap::iterator chunk_end) {
    for(auto chunk_it = chunk_begin; chunk_it != chunk_end; ++chunk_it) {
        Vector2i chunk_coords = chunk_it->first;
	    Chunk* chunk = &chunk_it->second;

        // auto modifications = chunk->pop_modifications();

        if(m_render_data.count(chunk_coords) == 0) {
            auto chunk_render = std::make_unique<ChunkRenderData>();
            chunk_render->generateChunkMesh(chunk);
            chunk_render->update_vertex_buffer();

            m_render_data[chunk_coords] = std::move(chunk_render);
        }
    }
}
