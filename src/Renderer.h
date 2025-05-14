#pragma once
#include <memory>
#include "Shader.h"
#include "TextureAtlas.h"

class World;
class Camera;

class Renderer {
public:
	Renderer();

	void render(World* world, Camera* camera);

private:
	std::unique_ptr<Shader> shader;
    std::unique_ptr<TextureAtlas> blockTextures;
	unsigned int dataSize;
};
