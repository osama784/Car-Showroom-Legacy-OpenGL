#include "ShowRoomHelper.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "TextureHelper.h"

ShowRoomHelper::ShowRoomHelper()
    : mainWidth(80.0f), mainHeight(20.0f), mainDepth(60.0f) {

    mainFloorTexture = 0;
    mainWallTexture = 0;
    mainCeilingTexture = 0;
    sectionFloorTexture = 0;
    sectionWallTexture = 0;

    srand(static_cast<unsigned>(time(NULL)));
}

ShowRoomHelper::~ShowRoomHelper() {
    // Cleanup if needed
}

void ShowRoomHelper::initialize(float width, float height, float depth) {
    mainWidth = width;
    mainHeight = height;
    mainDepth = depth;

    loadDefaultTextures();

    // 2x2 grid of sections
    createGridSections(2, 2, mainWidth / 3.2f, mainDepth / 2.2f);
}

void ShowRoomHelper::loadDefaultTextures() {
    // Try to load textures - adjust filenames as needed
    mainFloorTexture = TextureHelper::LoadTexture("showroom_floor.jpg");
    mainWallTexture = TextureHelper::LoadTexture("glass.jpg");
    mainCeilingTexture = TextureHelper::LoadTexture("ceiling.jpg");

    // Load different textures for internal sections
    sectionFloorTexture = TextureHelper::LoadTexture("section_floor.jpg");
    if (sectionFloorTexture == 0) {
        sectionFloorTexture = TextureHelper::LoadTexture("floor.png");  // Fallback
    }

    sectionWallTexture = TextureHelper::LoadTexture("section_wall.jpg");
    if (sectionWallTexture == 0) {
        sectionWallTexture = TextureHelper::LoadTexture("glass.jpg");  // Fallback
    }
}


void ShowRoomHelper::setMainShowroomSize(float width, float height, float depth) {
    mainWidth = width;
    mainHeight = height;
    mainDepth = depth;
}

void ShowRoomHelper::addSection(float x, float z, float width, float depth,
    float wallHeight, int carCapacity) {
    ShowroomSection section;
    section.x = x;
    section.z = z;
    section.width = width;
    section.depth = depth;
    section.wallHeight = wallHeight;
    section.floorTexture = sectionFloorTexture;
    section.wallTexture = sectionWallTexture;
    section.carCapacity = carCapacity;

    // Center the display platform within the section
    section.displayPlatformX = x;
    section.displayPlatformZ = z;
    section.displayPlatformWidth = width * 0.7f;
    section.displayPlatformDepth = depth * 0.8f;

    // Position light above the platform
    section.lightPosX = x;
    section.lightPosY = mainHeight - 5.0f;
    section.lightPosZ = z;

    sections.push_back(section);
}

void ShowRoomHelper::createGridSections(int rows, int cols, float sectionWidth,
    float sectionDepth) {
    sections.clear();

    float totalWidth = cols * sectionWidth;
    float totalDepth = rows * sectionDepth;

    // Center the grid in the main showroom
    float startX = -totalWidth / 2.0f + sectionWidth / 2.0f;
    float startZ = -totalDepth / 2.0f + sectionDepth / 2.0f;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            float x = startX + col * sectionWidth;
            float z = startZ + row * sectionDepth;

            addSection(x, z, sectionWidth * 0.9f, sectionDepth * 0.9f, 6.0f, 1);
        }
    }
}

void ShowRoomHelper::clearSections() {
    sections.clear();
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

void ShowRoomHelper::drawTexturedCube(float x, float y, float z,
    float width, float height, float depth,
    GLuint textureID, float texScale) {
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    float halfD = depth / 2.0f;

    glPushMatrix();
    glTranslatef(x, y, z);

    glBindTexture(GL_TEXTURE_2D, textureID);

    // Front
    DrawTexturedQuad(-halfW, -halfH, halfD,
        halfW, -halfH, halfD,
        halfW, halfH, halfD,
        -halfW, halfH, halfD);

    // Back
    DrawTexturedQuad(-halfW, -halfH, -halfD,
        halfW, -halfH, -halfD,
        halfW, halfH, -halfD,
        -halfW, halfH, -halfD);

    // Left
    DrawTexturedQuad(-halfW, -halfH, -halfD,
        -halfW, -halfH, halfD,
        -halfW, halfH, halfD,
        -halfW, halfH, -halfD);

    // Right
    DrawTexturedQuad(halfW, -halfH, -halfD,
        halfW, -halfH, halfD,
        halfW, halfH, halfD,
        halfW, halfH, -halfD);

    // Top
    DrawTexturedQuad(-halfW, halfH, -halfD,
        halfW, halfH, -halfD,
        halfW, halfH, halfD,
        -halfW, halfH, halfD);

    // Bottom
    DrawTexturedQuad(-halfW, -halfH, -halfD,
        halfW, -halfH, -halfD,
        halfW, -halfH, halfD,
        -halfW, -halfH, halfD);

    glPopMatrix();
}

void ShowRoomHelper::drawMainShowroom() {
    float halfW = mainWidth / 2.0f;
    float halfH = mainHeight / 2.0f;
    float halfD = mainDepth / 2.0f;

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // ----- Draw FLOOR (opaque) -----
    setFloorMaterial();
    glBindTexture(GL_TEXTURE_2D, mainFloorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        halfW, 0.0f, -halfD,
        halfW, 0.0f, halfD,
        -halfW, 0.0f, halfD);

    // ----- Draw GLASS WALLS and CEILING (transparent) -----
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setGlassMaterial();
    glBindTexture(GL_TEXTURE_2D, mainWallTexture);

    // Back wall
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        halfW, 0.0f, -halfD,
        halfW, mainHeight, -halfD,
        -halfW, mainHeight, -halfD);

    // Front wall (with entrance - leave middle part open)
    float entranceWidth = mainWidth * 0.3f;
    // Left part of front wall
    DrawTexturedQuad(-halfW, 0.0f, halfD,
        -entranceWidth / 2, 0.0f, halfD,
        -entranceWidth / 2, mainHeight, halfD,
        -halfW, mainHeight, halfD);
    // Right part of front wall
    DrawTexturedQuad(entranceWidth / 2, 0.0f, halfD,
        halfW, 0.0f, halfD,
        halfW, mainHeight, halfD,
        entranceWidth / 2, mainHeight, halfD);
    // Top part of front wall (over entrance)
    DrawTexturedQuad(-entranceWidth / 2, mainHeight * 0.7f, halfD,
        entranceWidth / 2, mainHeight * 0.7f, halfD,
        entranceWidth / 2, mainHeight, halfD,
        -entranceWidth / 2, mainHeight, halfD);

    // Left wall
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        -halfW, 0.0f, halfD,
        -halfW, mainHeight, halfD,
        -halfW, mainHeight, -halfD);

    // Right wall
    DrawTexturedQuad(halfW, 0.0f, -halfD,
        halfW, 0.0f, halfD,
        halfW, mainHeight, halfD,
        halfW, mainHeight, -halfD);

    // Ceiling
    glBindTexture(GL_TEXTURE_2D, mainCeilingTexture);
    DrawTexturedQuad(-halfW, mainHeight, -halfD,
        halfW, mainHeight, -halfD,
        halfW, mainHeight, halfD,
        -halfW, mainHeight, halfD);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    resetMaterial();
}

void ShowRoomHelper::drawSection(const ShowroomSection& section) {
    float halfW = section.width / 2.0f;
    float halfD = section.depth / 2.0f;

    // Draw section floor
    setFloorMaterial();
    glBindTexture(GL_TEXTURE_2D, section.floorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    DrawTexturedQuad(section.x - halfW, 0.0f, section.z - halfD,
        section.x + halfW, 0.0f, section.z - halfD,
        section.x + halfW, 0.0f, section.z + halfD,
        section.x - halfW, 0.0f, section.z + halfD);

    // Draw section walls (lower height, less opaque)
    
    glBindTexture(GL_TEXTURE_2D, section.wallTexture);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setGlassMaterial();

    // Only draw 3 walls, leave front open for viewing
    // Back wall
    DrawTexturedQuad(section.x - halfW, 0.0f, section.z - halfD,
        section.x + halfW, 0.0f, section.z - halfD,
        section.x + halfW, section.wallHeight, section.z - halfD,
        section.x - halfW, section.wallHeight, section.z - halfD);

    // Left wall
    DrawTexturedQuad(section.x - halfW, 0.0f, section.z - halfD,
        section.x - halfW, 0.0f, section.z + halfD,
        section.x - halfW, section.wallHeight, section.z + halfD,
        section.x - halfW, section.wallHeight, section.z - halfD);

    // Right wall
    DrawTexturedQuad(section.x + halfW, 0.0f, section.z - halfD,
        section.x + halfW, 0.0f, section.z + halfD,
        section.x + halfW, section.wallHeight, section.z + halfD,
        section.x + halfW, section.wallHeight, section.z - halfD);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    resetMaterial();
}

void ShowRoomHelper::drawAllSections() {
    for (const auto& section : sections) {
        drawSection(section);
        drawDisplayPlatform(section.displayPlatformX, section.displayPlatformZ,
            section.displayPlatformWidth, section.displayPlatformDepth);
    }
}

void ShowRoomHelper::drawShowroomComplete() {
    // Draw in correct order for transparency
    drawMainShowroom();      // Draw outer glass structure first
    drawAllSections();       // Then draw internal sections
    drawAllDisplayPlatforms(); // Then draw display platforms
}

void ShowRoomHelper::drawDisplayPlatform(float x, float z, float width, float depth, float height) {
    // Draw a raised platform for cars to sit on
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.9f, 0.9f, 0.9f);  // Light gray platform

    float halfW = width / 2.0f;
    float halfD = depth / 2.0f;

    // Top of platform
    glBegin(GL_QUADS);
    glVertex3f(x - halfW, height, z - halfD);
    glVertex3f(x + halfW, height, z - halfD);
    glVertex3f(x + halfW, height, z + halfD);
    glVertex3f(x - halfW, height, z + halfD);
    glEnd();

    // Sides of platform
    glColor3f(0.7f, 0.7f, 0.7f);

    // Front side
    glBegin(GL_QUADS);
    glVertex3f(x - halfW, 0.0f, z - halfD);
    glVertex3f(x + halfW, 0.0f, z - halfD);
    glVertex3f(x + halfW, height, z - halfD);
    glVertex3f(x - halfW, height, z - halfD);
    glEnd();

    // Back side
    glBegin(GL_QUADS);
    glVertex3f(x - halfW, 0.0f, z + halfD);
    glVertex3f(x + halfW, 0.0f, z + halfD);
    glVertex3f(x + halfW, height, z + halfD);
    glVertex3f(x - halfW, height, z + halfD);
    glEnd();

    // Left side
    glBegin(GL_QUADS);
    glVertex3f(x - halfW, 0.0f, z - halfD);
    glVertex3f(x - halfW, 0.0f, z + halfD);
    glVertex3f(x - halfW, height, z + halfD);
    glVertex3f(x - halfW, height, z - halfD);
    glEnd();

    // Right side
    glBegin(GL_QUADS);
    glVertex3f(x + halfW, 0.0f, z - halfD);
    glVertex3f(x + halfW, 0.0f, z + halfD);
    glVertex3f(x + halfW, height, z + halfD);
    glVertex3f(x + halfW, height, z - halfD);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ShowRoomHelper::drawAllDisplayPlatforms() {
    for (const auto& section : sections) {
        drawDisplayPlatform(section.displayPlatformX, section.displayPlatformZ,
            section.displayPlatformWidth, section.displayPlatformDepth);
    }
}

void ShowRoomHelper::setupShowroomLighting() {
    // Setup basic lighting for the showroom
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Main overhead lights
    float lightPos[] = { 0.0f, mainHeight - 2.0f, 0.0f, 1.0f };
    float lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Additional lights for each section
    for (size_t i = 0; i < sections.size(); i++) {
        if (i < 7) {  // up to 8 lights
            GLenum lightID = GL_LIGHT1 + i;
            glEnable(lightID);

            float sectionLightPos[] = {
                sections[i].lightPosX,
                sections[i].lightPosY,
                sections[i].lightPosZ,
                1.0f
            };

            float sectionLightDiffuse[] = { 0.9f, 0.9f, 0.7f, 1.0f };  // Slightly yellow

            glLightfv(lightID, GL_POSITION, sectionLightPos);
            glLightfv(lightID, GL_DIFFUSE, sectionLightDiffuse);
        }
    }
}

void ShowRoomHelper::setGlassMaterial() {
    glColor4f(0.9f, 0.95f, 1.0f, 0.3f);  // Light blue tint with 30% opacity

    float mat_specular[] = { 0.8f, 0.8f, 0.8f, 0.3f };
    float mat_shininess[] = { 100.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}


void ShowRoomHelper::setFloorMaterial() {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  // Opaque white

    float mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float mat_shininess[] = { 10.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void ShowRoomHelper::resetMaterial() {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    float default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float default_shininess[] = { 0.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, default_shininess);
}

void ShowRoomHelper::renderSectionInfo() {
    // For debugging - prints section info to console
    std::cout << "=== Showroom Sections ===" << std::endl;
    std::cout << "Main Showroom: " << mainWidth << " x " << mainHeight << " x " << mainDepth << std::endl;
    std::cout << "Number of sections: " << sections.size() << std::endl;

    for (size_t i = 0; i < sections.size(); i++) {
        std::cout << "Section " << i << ": ";
        std::cout << "Pos(" << sections[i].x << ", " << sections[i].z << ") ";
        std::cout << "Size(" << sections[i].width << " x " << sections[i].depth << ") ";
        std::cout << "Cars: " << sections[i].carCapacity << std::endl;
    }
}