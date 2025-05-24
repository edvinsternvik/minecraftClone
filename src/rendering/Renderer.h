#pragma once
#include <memory>
#include "ChunkRenderer.h"
#include "Shader.h"
#include "TextureAtlas.h"

class World;
class Camera;

class Renderer {
public:
    Renderer();

    void render(World* world, Camera* camera);
    void prepare_chunks(World* world);

private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<TextureAtlas> blockTextures;
    std::unique_ptr<ChunkRenderer> m_chunk_renderer;
    unsigned int dataSize;
};
