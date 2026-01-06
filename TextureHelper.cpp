#include "TextureHelper.h"

#include <cmath>
#include <iostream>

GLuint TextureHelper::LoadTexture(const char* filename) {
    int width, height, channels;

    // Flip image vertically for OpenGL coordinate system
    stbi_set_flip_vertically_on_load(true);

    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

    if (!image) {
        std::cout << "TextureHelper: Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Determine format based on channels
    GLenum format = GL_RGB;
    if (channels == 4)
        format = GL_RGBA;
    else if (channels == 1)
        format = GL_LUMINANCE;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
        format, GL_UNSIGNED_BYTE, image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(image);
    stbi_set_flip_vertically_on_load(false);

    std::cout << "TextureHelper: Loaded texture " << filename
        << " (" << width << "x" << height << ")" << std::endl;

    return textureID;
}