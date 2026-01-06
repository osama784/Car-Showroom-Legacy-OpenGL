#include "ShowRoomHelper.h"
#include <cmath>
#include <iostream>
#include "TextureHelper.h"

// STB Image for texture loading
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

ShowRoomHelper::ShowRoomHelper() {
    // Constructor can initialize anything if needed
    floorTexture = 0;
    wallTexture = 0;
    ceilingTexture = 0;
 
}

ShowRoomHelper::~ShowRoomHelper() {
    // Cleanup if needed
}

void ShowRoomHelper::initialize() {
    loadDefaultTextures();

}

void ShowRoomHelper::loadDefaultTextures() {
    wallTexture = TextureHelper::LoadTexture("glass.jpg");
    floorTexture = TextureHelper::LoadTexture("floor.png");
    ceilingTexture = TextureHelper::LoadTexture("glass.jpg");
}


void ShowRoomHelper::DrawTexturedQuad(float x1, float y1, float z1,
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

void ShowRoomHelper::DrawRoom(float roomWidth, float roomHeight, float roomDepth) {
    float halfW = roomWidth / 2.0f;
    float halfH = roomHeight / 2.0f;
    float halfD = roomDepth / 2.0f;

    // floor
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        halfW, 0.0f, -halfD,
        halfW, 0.0f, halfD,
        -halfW, 0.0f, halfD);

    
    glDepthMask(GL_FALSE);

    setGlassMaterial();

    // back wall
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        halfW, 0.0f, -halfD,
        halfW, roomHeight, -halfD,
        -halfW, roomHeight, -halfD);

    // left wall
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        -halfW, 0.0f, halfD,
        -halfW, roomHeight, halfD,
        -halfW, roomHeight, -halfD);

    // right wall
    DrawTexturedQuad(halfW, 0.0f, -halfD,
        halfW, 0.0f, halfD,
        halfW, roomHeight, halfD,
        halfW, roomHeight, -halfD);

    // ceiling
    glBindTexture(GL_TEXTURE_2D, ceilingTexture);
    DrawTexturedQuad(-halfW, roomHeight, -halfD,
        halfW, roomHeight, -halfD,
        halfW, roomHeight, halfD,
        -halfW, roomHeight, halfD);


    // Re-enable depth buffer writing
    glDepthMask(GL_TRUE);
    resetMaterial();
}

void ShowRoomHelper::RenderText(float x, float y, const char* text) {
    // Simple text rendering using GLUT (if available)
    // For now, we'll leave this as a stub to implement later
    // You can use GLUT's glutBitmapCharacter or Windows GDI
}

void ShowRoomHelper::EnableTexturing(bool enable) {
    if (enable) {
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
}


void ShowRoomHelper::setGlassMaterial() {
    // Set glass-like properties
    glColor4f(0.9f, 0.95f, 1.0f, 0.3f);  // Light blue tint with 30% opacity

    // Optional: Add some specular highlights for glass effect
    float mat_specular[] = { 0.8f, 0.8f, 0.8f, 0.3f };
    float mat_shininess[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void ShowRoomHelper::resetMaterial() {
    // Reset to default material
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    float default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float default_shininess[] = { 0.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, default_shininess);
}