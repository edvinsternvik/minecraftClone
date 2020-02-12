#include "Renderer.h"

#include <GL/glew.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "World.h"
#include "ChunkRenderer.h"
#include "Camera.h"

#include <vector>

Renderer::Renderer() {
	if(glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize glew" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// const float crosshairVertexBufferData[] = {
	// 	-0.5, -0.5, 0.0,
	// 	0.5, -0.5, 0.0,
	// 	0.5, 0.5, 0.0
	// };
	// const int crosshairVertexBufferAttributes[] = {3};
	// crosshairVertexBuffer = std::make_unique<VertexBuffer>(crosshairVertexBufferData, 9, crosshairVertexBufferAttributes, 1);
	// otherShader = std::make_unique<Shader>("assets/shaders/otherVertexShader.glsl", "assets/shaders/otherFragmentShader.glsl");

	shader = std::make_unique<Shader>("assets/shaders/vertexShader.glsl", "assets/shaders/fragmentShader.glsl");
	shader->useShader();

	glm::mat4 projectionMat = glm::perspective(45.0, 1280.0 / 720.0, 0.1, 100.0);
	glm::mat4 viewMat = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -2.0));

	shader->setUniformMatrix4f("u_projection", &projectionMat[0][0]);
	shader->setUniformMatrix4f("u_view", &viewMat[0][0]);
	shader->setUniformMatrix4f("u_model", &modelMat[0][0]);

	blockTextures = new TextureAtlas("assets/textures/Textures.png", 16, 16);
	blockTextures->bind();

	glClearColor(0.1, 0.6, 0.9, 1.0);

	// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void Renderer::render(World* world) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->useShader();
	if(m_camera) {
		shader->setUniformMatrix4f("u_view", &m_camera->viewMatrix[0][0]);
	}
	
	Chunk* chunk = world->chunks;
	while(chunk) {
		ChunkRenderer* chunkRenderer = chunk->getChunkRenderer();

		if(chunkRenderer->getChunkMeshVerticiesCount() > 0) {
			chunkRenderer->bind();

			if(chunkRenderer->updatedChunkMesh) {
				
				chunkRenderer->m_vertexBuffer->setDataSize(chunkRenderer->getChunkMeshVerticiesCount());

				int offset = 0;
				for(int i = 0; i < CHUNK_SEGMENTS; ++i) {
					int vertexCount = chunkRenderer->getChunkSegmentMeshVerticiesCount(i);
					chunkRenderer->m_vertexBuffer->setSubData(chunkRenderer->getChunkSegmentData(i), vertexCount, offset);
					offset += vertexCount;
				}
				chunkRenderer->updatedChunkMesh = false;
			}

			glDrawArrays(GL_TRIANGLES, 0, chunkRenderer->getChunkMeshVerticiesCount());
		}

		chunk = chunk->next;
	}
}

void Renderer::setCamera(Camera* camera) {
	m_camera = camera;
}