#pragma once

class Texture {
public:
    Texture(const char* filePath);
    ~Texture();

    inline int getWidth() { return imageWidth; }
    inline int getHeight() { return imageHeight; }

    void bind();
    void unbind();

private:
    int imageWidth, imageHeight, n;
    unsigned int texture;
};
