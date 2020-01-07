#include "TextureAtlas.h"
#include "Shader.h"

TextureAtlas::TextureAtlas(const char* filePath, int spriteWidth, int spriteHeight) : texture(filePath), 
		spriteWidth(spriteWidth), spriteHeight(spriteHeight), texturesX(texture.getWidth() / spriteWidth), texturesY(texture.getHeight() / spriteHeight) {

}

void TextureAtlas::bind() {
	Shader* currentShader = Shader::getCurrentShader();
	if(currentShader) {
		currentShader->setUniform2f("u_textureSize", 1.0 / (float)texturesX, 1.0 / (float)texturesY);
	}

	texture.bind();
}

void TextureAtlas::unbind() {
	texture.unbind();
}