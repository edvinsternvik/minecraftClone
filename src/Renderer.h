#pragma once
#include <memory>
#include "VertexBuffer.h"
#include "Shader.h"
#include "TextureAtlas.h"

#include "Math.h"

class World;
class Camera;

class Renderer {
public:
	Renderer();

	void render(World* world);

	void setCamera(Camera* camera);

private:
	Camera* m_camera = nullptr;
	std::unique_ptr<VertexBuffer> chunkVertexBuffer;
	std::unique_ptr<Shader> shader;
	TextureAtlas* blockTextures;
	unsigned int dataSize;
	// std::unique_ptr<VertexBuffer> crosshairVertexBuffer;
	// std::unique_ptr<Shader> otherShader;
};