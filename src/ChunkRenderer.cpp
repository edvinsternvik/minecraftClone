#include "ChunkRenderer.h"
#include "Application.h"

ChunkRenderer::ChunkRenderer(Chunk* chunk) : m_chunk(chunk), chunkMeshSize(0) {
	m_vertexArray = new VertexArray();
	m_vertexArray->bind();

	const int chunkVertexBufferAttributes[] = {1};
	m_vertexBuffer = new VertexBuffer(chunkVertexBufferAttributes, 1);
	m_vertexBuffer->unbind();
	m_vertexArray->unbind();
}

ChunkRenderer::~ChunkRenderer() {
	delete m_vertexArray;
	delete m_vertexBuffer;
}

void ChunkRenderer::generateChunkMesh() {
	for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
		generateChunkSegmentMesh(i);
	}

	chunkMeshSize = 0;
	for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
		chunkMeshSize += getChunkSegmentMeshVerticiesCount(i);
	}
}

void ChunkRenderer::generateChunkSegmentMesh(int index){
	if(index < 0 || index >= CHUNK_SEGMENTS) return;

	auto* segmentMesh = &chunkMesh[index];

	segmentMesh->resize(0);
	int chunkWorldX = m_chunk->chunkX * CHUNK_WIDTH;
	int chunkWorldZ = m_chunk->chunkZ * CHUNK_WIDTH;

	World* world = Application::getInstance().getWorld();

	int yOffset = index * CHUNK_SEGMENT_HEIGHT;
	for(int x = 0; x < CHUNK_WIDTH; ++x) {
		for(int y = yOffset; y < CHUNK_SEGMENT_HEIGHT + yOffset; ++y) {
			for(int z = 0; z < CHUNK_WIDTH; ++z) {
				const Block* block = &m_chunk->getBlock(x, y, z);
				int worldX = x + chunkWorldX, worldZ = z + chunkWorldZ;

				if(block->getBlockType() == BlockType::Solid) {

					if(!world->isSolid(worldX, y, worldZ + 1)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Front, Vector3(x, y, z), block->getBlockId()));
					if(!world->isSolid(worldX, y, worldZ - 1)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Back , Vector3(x, y, z), block->getBlockId()));
					if(!world->isSolid(worldX - 1, y, worldZ)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Left , Vector3(x, y, z), block->getBlockId()));
					if(!world->isSolid(worldX + 1, y, worldZ)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Right, Vector3(x, y, z), block->getBlockId()));
					if(!world->isSolid(worldX, y + 1, worldZ)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Top  , Vector3(x, y, z), block->getBlockId()));
					if(!world->isSolid(worldX, y - 1, worldZ)) segmentMesh->push_back(BlockModelData::getBlockData(BlockSide::Down , Vector3(x, y, z), block->getBlockId()));
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

void ChunkRenderer::updateChunkMesh(int x, int y, int z){
	Vector4i segPos = m_chunk->calculateSegmentPosFromChunkPos(x, y, z);
	generateChunkSegmentMesh(segPos.w);

	int chunkSegmentY = y % CHUNK_SEGMENT_HEIGHT;
	if(chunkSegmentY == 0 && segPos.w > 0) generateChunkSegmentMesh(segPos.w - 1);
	if(chunkSegmentY == CHUNK_SEGMENT_HEIGHT - 1 && segPos.w < CHUNK_SEGMENTS - 1) generateChunkSegmentMesh(segPos.w + 1);
}

void ChunkRenderer::bind() {
	m_vertexArray->bind();
	m_vertexBuffer->bind();
}