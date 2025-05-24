#pragma once
#include "Texture.h"

class TextureAtlas {
public:
    TextureAtlas(const char* filePath, int spriteWidth, int spriteHeight);

    inline int getTextureXCoords(int textureNumberX) { return spriteWidth * textureNumberX; }
    inline int getTextureYCoords(int textureNumberY) { return spriteHeight * textureNumberY; }

    void bind();
    void unbind();

private:
    Texture texture;
    int spriteWidth, spriteHeight;
    int texturesX, texturesY;
};