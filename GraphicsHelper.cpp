#include "GraphicsHelper.h"
#include <cmath>
#include <iostream>

// STB Image for texture loading
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

GraphicsHelper::GraphicsHelper() {
    // Constructor can initialize anything if needed
}

GraphicsHelper::~GraphicsHelper() {
    // Cleanup if needed
}

GLuint GraphicsHelper::LoadTexture(const char* filename) {
    int width, height, channels;

    // Flip image vertically for OpenGL coordinate system
    stbi_set_flip_vertically_on_load(true);

    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

    if (!image) {
        std::cout << "GraphicsHelper: Failed to load texture: " << filename << std::endl;
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

    std::cout << "GraphicsHelper: Loaded texture " << filename
        << " (" << width << "x" << height << ")" << std::endl;

    return textureID;
}

void GraphicsHelper::DrawTexturedQuad(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float x4, float y4, float z4) {
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, z1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, y2, z2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x3, y3, z3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x4, y4, z4);
    glEnd();
}

void GraphicsHelper::DrawColoredQuad(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float x4, float y4, float z4,
    float r, float g, float b) {
    EnableTexturing(false);
    glColor3f(r, g, b);

    glBegin(GL_QUADS);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    glEnd();

    EnableTexturing(true);
    ResetColor();
}

void GraphicsHelper::DrawCube(float x, float y, float z,
    float width, float height, float depth,
    GLuint textureID) {
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    float halfD = depth / 2.0f;

    glPushMatrix();
    glTranslatef(x, y, z);

    if (textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    // Front face
    DrawTexturedQuad(-halfW, -halfH, halfD,
        halfW, -halfH, halfD,
        halfW, halfH, halfD,
        -halfW, halfH, halfD);

    // Back face
    DrawTexturedQuad(-halfW, -halfH, -halfD,
        halfW, -halfH, -halfD,
        halfW, halfH, -halfD,
        -halfW, halfH, -halfD);

    // Left face
    DrawTexturedQuad(-halfW, -halfH, -halfD,
        -halfW, -halfH, halfD,
        -halfW, halfH, halfD,
        -halfW, halfH, -halfD);

    // Right face
    DrawTexturedQuad(halfW, -halfH, -halfD,
        halfW, -halfH, halfD,
        halfW, halfH, halfD,
        halfW, halfH, -halfD);

    // Top face
    DrawTexturedQuad(-halfW, halfH, -halfD,
        halfW, halfH, -halfD,
        halfW, halfH, halfD,
        -halfW, halfH, halfD);

    // Bottom face
    DrawTexturedQuad(-halfW, -halfH, -halfD,
        halfW, -halfH, -halfD,
        halfW, -halfH, halfD,
        -halfW, -halfH, halfD);

    glPopMatrix();
}

void GraphicsHelper::DrawWall(float x, float y, float z,
    float width, float height, bool frontBack,
    GLuint textureID) {
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;

    if (textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    if (frontBack) {
        // Front/back wall (facing Z axis)
        DrawTexturedQuad(x - halfW, y - halfH, z,
            x + halfW, y - halfH, z,
            x + halfW, y + halfH, z,
            x - halfW, y + halfH, z);
    }
    else {
        // Side wall (facing X axis)
        DrawTexturedQuad(x, y - halfH, z - halfW,
            x, y - halfH, z + halfW,
            x, y + halfH, z + halfW,
            x, y + halfH, z - halfW);
    }
}

void GraphicsHelper::DrawGrid(float size, int divisions, float y) {
    EnableTexturing(false);
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(1.0f);

    float halfSize = size / 2.0f;
    float step = size / divisions;

    glBegin(GL_LINES);
    for (float x = -halfSize; x <= halfSize; x += step) {
        glVertex3f(x, y, -halfSize);
        glVertex3f(x, y, halfSize);
    }
    for (float z = -halfSize; z <= halfSize; z += step) {
        glVertex3f(-halfSize, y, z);
        glVertex3f(halfSize, y, z);
    }
    glEnd();

    // Draw main axes
    glColor3f(1.0f, 0.0f, 0.0f); // X - Red
    glBegin(GL_LINES);
    glVertex3f(0.0f, y, 0.0f);
    glVertex3f(size / 3, y, 0.0f);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f); // Y - Green
    glBegin(GL_LINES);
    glVertex3f(0.0f, y, 0.0f);
    glVertex3f(0.0f, y + size / 3, 0.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f); // Z - Blue
    glBegin(GL_LINES);
    glVertex3f(0.0f, y, 0.0f);
    glVertex3f(0.0f, y, size / 3);
    glEnd();

    EnableTexturing(true);
    ResetColor();
}

void GraphicsHelper::DrawAxis(float length) {
    EnableTexturing(false);
    glLineWidth(2.0f);

    glBegin(GL_LINES);
    // X axis - Red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);

    // Y axis - Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);

    // Z axis - Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, length);
    glEnd();

    EnableTexturing(true);
    ResetColor();
}

void GraphicsHelper::DrawRoom(float roomWidth, float roomHeight, float roomDepth,
    GLuint wallTexture, GLuint floorTexture, GLuint ceilingTexture) {
    glColor3f(1.0f, 1.0f, 1.0f);  // White color for full texture brightness

    float halfW = roomWidth / 2.0f;
    float halfH = roomHeight / 2.0f;
    float halfD = roomDepth / 2.0f;

    // Draw floor
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        halfW, 0.0f, -halfD,
        halfW, 0.0f, halfD,
        -halfW, 0.0f, halfD);

    // Draw ceiling
    glBindTexture(GL_TEXTURE_2D, ceilingTexture);
    DrawTexturedQuad(-halfW, roomHeight, -halfD,
        halfW, roomHeight, -halfD,
        halfW, roomHeight, halfD,
        -halfW, roomHeight, halfD);

    // Draw walls
    glBindTexture(GL_TEXTURE_2D, wallTexture);

    // Back wall
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        halfW, 0.0f, -halfD,
        halfW, roomHeight, -halfD,
        -halfW, roomHeight, -halfD);

    // Front wall
    /*DrawTexturedQuad(-halfW, 0.0f, halfD,
        halfW, 0.0f, halfD,
        halfW, roomHeight, halfD,
        -halfW, roomHeight, halfD);*/

    // Left wall
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        -halfW, 0.0f, halfD,
        -halfW, roomHeight, halfD,
        -halfW, roomHeight, -halfD);

    // Right wall
    DrawTexturedQuad(halfW, 0.0f, -halfD,
        halfW, 0.0f, halfD,
        halfW, roomHeight, halfD,
        halfW, roomHeight, -halfD);
}

void GraphicsHelper::DrawWireframeRoom(float roomWidth, float roomHeight, float roomDepth) {
    EnableTexturing(false);
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(1.0f);

    float halfW = roomWidth / 2.0f;
    float halfH = roomHeight / 2.0f;
    float halfD = roomDepth / 2.0f;

    glBegin(GL_LINES);
    // Floor outline
    glVertex3f(-halfW, 0.0f, -halfD); glVertex3f(halfW, 0.0f, -halfD);
    glVertex3f(halfW, 0.0f, -halfD); glVertex3f(halfW, 0.0f, halfD);
    glVertex3f(halfW, 0.0f, halfD); glVertex3f(-halfW, 0.0f, halfD);
    glVertex3f(-halfW, 0.0f, halfD); glVertex3f(-halfW, 0.0f, -halfD);

    // Ceiling outline
    glVertex3f(-halfW, roomHeight, -halfD); glVertex3f(halfW, roomHeight, -halfD);
    glVertex3f(halfW, roomHeight, -halfD); glVertex3f(halfW, roomHeight, halfD);
    glVertex3f(halfW, roomHeight, halfD); glVertex3f(-halfW, roomHeight, halfD);
    glVertex3f(-halfW, roomHeight, halfD); glVertex3f(-halfW, roomHeight, -halfD);

    // Vertical edges
    glVertex3f(-halfW, 0.0f, -halfD); glVertex3f(-halfW, roomHeight, -halfD);
    glVertex3f(halfW, 0.0f, -halfD); glVertex3f(halfW, roomHeight, -halfD);
    glVertex3f(halfW, 0.0f, halfD); glVertex3f(halfW, roomHeight, halfD);
    glVertex3f(-halfW, 0.0f, halfD); glVertex3f(-halfW, roomHeight, halfD);
    glEnd();

    EnableTexturing(true);
    ResetColor();
}

void GraphicsHelper::RenderText(float x, float y, const char* text) {
    // Simple text rendering using GLUT (if available)
    // For now, we'll leave this as a stub to implement later
    // You can use GLUT's glutBitmapCharacter or Windows GDI
}

void GraphicsHelper::SetColor(float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
}

void GraphicsHelper::ResetColor() {
    glColor3f(1.0f, 1.0f, 1.0f); // Reset to white
}

void GraphicsHelper::EnableTexturing(bool enable) {
    if (enable) {
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
}