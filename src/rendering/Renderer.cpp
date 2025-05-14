#include "Renderer.h"

#include <GL/glew.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../world/World.h"
#include "ChunkRenderer.h"
#include "../Camera.h"

Renderer::Renderer() {
	if(glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize glew" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shader = std::make_unique<Shader>("assets/shaders/vertexShader.glsl", "assets/shaders/fragmentShader.glsl");
	shader->useShader();

	glm::mat4 projectionMat = glm::perspective(45.0, 1280.0 / 720.0, 0.1, 512.0);
	glm::mat4 viewMat = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -2.0));

	shader->setUniformMatrix4f("u_projection", &projectionMat[0][0]);
	shader->setUniformMatrix4f("u_view", &viewMat[0][0]);
	shader->setUniformMatrix4f("u_model", &modelMat[0][0]);

	blockTextures = std::make_unique<TextureAtlas>("assets/textures/Textures.png", 16, 16);
	blockTextures->bind();

	glClearColor(0.1, 0.6, 0.9, 1.0);

    m_chunk_renderer = std::make_unique<ChunkRenderer>();
}

void Renderer::render(World* world, Camera* camera) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->useShader();
	if(camera) {
		shader->setUniformMatrix4f("u_view", &camera->viewMatrix[0][0]);
	}
	
    for(auto chunk_it = m_chunk_renderer->chunk_render_data_begin(); chunk_it != m_chunk_renderer->chunk_render_data_end(); ++chunk_it) {
	    Vector2i chunk_pos = chunk_it->first;
	    ChunkRenderData* chunk = chunk_it->second.get();

		if(chunk->getChunkMeshVerticiesCount() > 0) {
			chunk->bind();

			shader->setUniform2i("u_chunkPosition", chunk_pos.x, chunk_pos.y);
			glDrawArrays(GL_TRIANGLES, 0, chunk->getChunkMeshVerticiesCount());
		}
	}
}

void Renderer::prepare_chunks(World* world) {
    m_chunk_renderer->prepare_chunks(world->get_chunk_map());
}
