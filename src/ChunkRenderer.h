#pragma once
#include "Chunk.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <vector>
#include <array>

class ChunkRenderer {
public:
	ChunkRenderer(Chunk* chunk);
	~ChunkRenderer();

	void generateChunkMesh();
	void generateChunkSegmentMesh(int index);
	void updateChunkMesh(int x, int y, int z);

private:

	inline unsigned int* getChunkSegmentData(int index) { return &chunkMesh[index][0][0]; }
	inline int getChunkSegmentMeshVerticiesCount(int index) const { return chunkMesh[index].size() * 6; }
	inline int getChunkMeshVerticiesCount() const { return chunkMeshSize; }
	bool isSolid(const int& localX, const int& y, const int& localZ, const int& chunkWorldX, const int& chunkWorldY, const Chunk* chunk);

	void bind();

private:
	const Chunk* const m_chunk;

	std::array<std::vector<std::array<unsigned int, 6>>, CHUNK_SEGMENTS> chunkMesh;
	int chunkMeshSize;

	VertexArray* m_vertexArray;
	VertexBuffer* m_vertexBuffer;
	bool updatedChunkMesh;

	friend class Renderer;
};