#include "ShowRoomHelper.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "TextureHelper.h"
#include "FurnitureHelper.h"

ShowRoomHelper::ShowRoomHelper()
    : mainWidth(80.0f), mainHeight(20.0f), mainDepth(60.0f) {

    mainFloorTexture = 0;
    mainWallTexture = 0;
    mainCeilingTexture = 0;
    sectionFloorTexture = 0;
    sectionWallTexture = 0;
    glassTexture = 0;  // ADD THIS LINE

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

    // Initialize furniture helper
    FurnitureHelper::initialize();

    // 2x2 grid of sections
    createGridSections(2, 2, mainWidth / 1.8f, mainDepth / 1.8f);
}

void ShowRoomHelper::loadDefaultTextures() {
    // Load main textures
    mainFloorTexture = TextureHelper::LoadTexture("marble.jpg");
    mainWallTexture = TextureHelper::LoadTexture("brickwhite.jpg");
    mainCeilingTexture = TextureHelper::LoadTexture("marble.jpg");

    // Load glass texture ONCE here
    glassTexture = TextureHelper::LoadTexture("glass.jpg");

    // Load different floor textures for each internal section
    sectionFloorTextures.clear();
    sectionFloorTextures.push_back(TextureHelper::LoadTexture("cobblestone.jpg"));
    sectionFloorTextures.push_back(TextureHelper::LoadTexture("marble.jpg"));
    sectionFloorTextures.push_back(TextureHelper::LoadTexture("floor.png"));
    sectionFloorTextures.push_back(TextureHelper::LoadTexture("brick.jpg"));

    // Fallback if any texture fails to load
    for (auto& tex : sectionFloorTextures) {
        if (tex == 0) {
            tex = TextureHelper::LoadTexture("floor.jpg");
        }
    }

    sectionWallTexture = TextureHelper::LoadTexture("brick.jpg");
}

void ShowRoomHelper::setMainShowroomSize(float width, float height, float depth) {
    mainWidth = width;
    mainHeight = height;
    mainDepth = depth;
}

void ShowRoomHelper::addSection(float x, float z, float width, float depth,
    float wallHeight, int carCapacity, int textureIndex) {
    ShowroomSection section;
    section.x = x;
    section.z = z;
    section.width = width;
    section.depth = depth;
    section.wallHeight = wallHeight;
    section.textureIndex = textureIndex;
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

            // Reduce section size further for wider paths (changed from 0.7f to 0.6f)
            addSection(x, z, sectionWidth * 0.6f, sectionDepth * 0.6f, 6.0f, 1);
        }
    }
}
void ShowRoomHelper::clearSections() {
    sections.clear();
}

void ShowRoomHelper::setSolidWallMaterial() {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  // Opaque

    float mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float mat_shininess[] = { 20.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void ShowRoomHelper::DrawTexturedQuad(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float x4, float y4, float z4) {
    // Calculate normal vector from the first 3 vertices
    float v1x = x2 - x1;
    float v1y = y2 - y1;
    float v1z = z2 - z1;

    float v2x = x3 - x1;
    float v2y = y3 - y1;
    float v2z = z3 - z1;

    // Cross product to get normal
    float nx = v1y * v2z - v1z * v2y;
    float ny = v1z * v2x - v1x * v2z;
    float nz = v1x * v2y - v1y * v2x;

    // Normalize the normal vector
    float length = sqrt(nx * nx + ny * ny + nz * nz);
    if (length > 0.0f) {
        nx /= length;
        ny /= length;
        nz /= length;
    }

    glBegin(GL_QUADS);
    glNormal3f(nx, ny, nz);  // ADD THIS LINE - sets the normal for the face
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

    glEnable(GL_TEXTURE_2D);

    // 1. Draw floor
    setFloorMaterial();
    glBindTexture(GL_TEXTURE_2D, mainFloorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        halfW, 0.0f, -halfD,
        halfW, 0.0f, halfD,
        -halfW, 0.0f, halfD);

    // 2. Draw solid walls (bottom and top parts)
    glDisable(GL_BLEND);
    setSolidWallMaterial();
    glBindTexture(GL_TEXTURE_2D, mainWallTexture);

    float wallBottom = 4.0;
    float windowHeight = 8.0f;

    // ----- BACK WALL -----
    // Back wall bottom
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        halfW, 0.0f, -halfD,
        halfW, wallBottom, -halfD,
        -halfW, wallBottom, -halfD);
    // Back wall top
    DrawTexturedQuad(-halfW, wallBottom + windowHeight, -halfD,
        halfW, wallBottom + windowHeight, -halfD,
        halfW, mainHeight, -halfD,
        -halfW, mainHeight, -halfD);

    // ----- FRONT WALL (with entrance) -----
    float entranceWidth = mainWidth * 0.3f;

    // Left part of front wall - bottom
    DrawTexturedQuad(-halfW, 0.0f, halfD,
        -entranceWidth / 2, 0.0f, halfD,
        -entranceWidth / 2, wallBottom, halfD,
        -halfW, wallBottom, halfD);
    // Left part of front wall - top
    DrawTexturedQuad(-halfW, wallBottom + windowHeight, halfD,
        -entranceWidth / 2, wallBottom + windowHeight, halfD,
        -entranceWidth / 2, mainHeight, halfD,
        -halfW, mainHeight, halfD);

    // Right part of front wall - bottom
    DrawTexturedQuad(entranceWidth / 2, 0.0f, halfD,
        halfW, 0.0f, halfD,
        halfW, wallBottom, halfD,
        entranceWidth / 2, wallBottom, halfD);
    // Right part of front wall - top
    DrawTexturedQuad(entranceWidth / 2, wallBottom + windowHeight, halfD,
        halfW, wallBottom + windowHeight, halfD,
        halfW, mainHeight, halfD,
        entranceWidth / 2, mainHeight, halfD);

    // Top part above entrance
    DrawTexturedQuad(-entranceWidth / 2, wallBottom + windowHeight, halfD,
        entranceWidth / 2, wallBottom + windowHeight, halfD,
        entranceWidth / 2, mainHeight, halfD,
        -entranceWidth / 2, mainHeight, halfD);

    // ----- LEFT WALL -----
    // Left wall - bottom
    DrawTexturedQuad(-halfW, 0.0f, -halfD,
        -halfW, 0.0f, halfD,
        -halfW, wallBottom, halfD,
        -halfW, wallBottom, -halfD);
    // Left wall - top
    DrawTexturedQuad(-halfW, wallBottom + windowHeight, -halfD,
        -halfW, wallBottom + windowHeight, halfD,
        -halfW, mainHeight, halfD,
        -halfW, mainHeight, -halfD);

    // ----- RIGHT WALL -----
    // Right wall - bottom
    DrawTexturedQuad(halfW, 0.0f, -halfD,
        halfW, 0.0f, halfD,
        halfW, wallBottom, halfD,
        halfW, wallBottom, -halfD);
    // Right wall - top
    DrawTexturedQuad(halfW, wallBottom + windowHeight, -halfD,
        halfW, wallBottom + windowHeight, halfD,
        halfW, mainHeight, halfD,
        halfW, mainHeight, -halfD);

    // 3. Draw transparent windows
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setGlassMaterial();
    glBindTexture(GL_TEXTURE_2D, glassTexture);

    // Back wall window
    DrawTexturedQuad(-halfW, wallBottom, -halfD,
        halfW, wallBottom, -halfD,
        halfW, wallBottom + windowHeight, -halfD,
        -halfW, wallBottom + windowHeight, -halfD);

    // Front wall left window
    DrawTexturedQuad(-halfW, wallBottom, halfD,
        -entranceWidth / 2, wallBottom, halfD,
        -entranceWidth / 2, wallBottom + windowHeight, halfD,
        -halfW, wallBottom + windowHeight, halfD);

    // Front wall right window
    DrawTexturedQuad(entranceWidth / 2, wallBottom, halfD,
        halfW, wallBottom, halfD,
        halfW, wallBottom + windowHeight, halfD,
        entranceWidth / 2, wallBottom + windowHeight, halfD);

    // Left wall window
    DrawTexturedQuad(-halfW, wallBottom, -halfD,
        -halfW, wallBottom, halfD,
        -halfW, wallBottom + windowHeight, halfD,
        -halfW, wallBottom + windowHeight, -halfD);

    // Right wall window
    DrawTexturedQuad(halfW, wallBottom, -halfD,
        halfW, wallBottom, halfD,
        halfW, wallBottom + windowHeight, halfD,
        halfW, wallBottom + windowHeight, -halfD);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    // 4. Draw ceiling
    glBindTexture(GL_TEXTURE_2D, mainCeilingTexture);
    DrawTexturedQuad(-halfW, mainHeight, -halfD,
        halfW, mainHeight, -halfD,
        halfW, mainHeight, halfD,
        -halfW, mainHeight, halfD);

    resetMaterial();
}

void ShowRoomHelper::drawSection(const ShowroomSection& section) {
    float halfW = section.width / 2.0f;
    float halfD = section.depth / 2.0f;
    glEnable(GL_NORMALIZE);
    // Draw section floor (use different texture based on index)
    setFloorMaterial();
    if (!sectionFloorTextures.empty()) {
        // Calculate texture index based on section's position
        size_t index = 0;
        for (size_t i = 0; i < sections.size(); i++) {
            if (&sections[i] == &section) {
                index = i % sectionFloorTextures.size();
                break;
            }
        }
        glBindTexture(GL_TEXTURE_2D, sectionFloorTextures[index]);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, sectionFloorTexture);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    DrawTexturedQuad(section.x - halfW, 0.0f, section.z - halfD,
        section.x + halfW, 0.0f, section.z - halfD,
        section.x + halfW, 0.0f, section.z + halfD,
        section.x - halfW, 0.0f, section.z + halfD);

    // ===== DRAW SOLID WALL PARTS (bottom and top) =====
    glDisable(GL_BLEND);  // Solid walls first
    setSolidWallMaterial();

    // Wall dimensions
    float sectionWallBottom = 1.2f;    // Solid bottom part height
    float sectionWindowHeight = 12.0f;  // Window height
    float sectionWallTop = section.wallHeight - sectionWallBottom - sectionWindowHeight;

    // Door width - leave space for door in front wall
    float doorWidth = section.width * 0.4f;
    bool doorOnPositiveZ = (section.z < 0);
    float doorZ = doorOnPositiveZ ? (section.z + halfD) : (section.z - halfD);
    float backZ = doorOnPositiveZ ? (section.z - halfD) : (section.z + halfD);

    // ---- BACK WALL (Now dynamic based on symmetry) ----
    // This is the wall WITHOUT the door
    DrawTexturedQuad(section.x - halfW, 0.0f, backZ,
        section.x + halfW, 0.0f, backZ,
        section.x + halfW, sectionWallBottom, backZ,
        section.x - halfW, sectionWallBottom, backZ);
    DrawTexturedQuad(section.x - halfW, sectionWallBottom + sectionWindowHeight, backZ,
        section.x + halfW, sectionWallBottom + sectionWindowHeight, backZ,
        section.x + halfW, section.wallHeight, backZ,
        section.x - halfW, section.wallHeight, backZ);

    // ---- LEFT WALL (full wall with window) ----
    // Bottom solid part
    DrawTexturedQuad(section.x - halfW, 0.0f, section.z - halfD,
        section.x - halfW, 0.0f, section.z + halfD,
        section.x - halfW, sectionWallBottom, section.z + halfD,
        section.x - halfW, sectionWallBottom, section.z - halfD);
    // Top solid part
    DrawTexturedQuad(section.x - halfW, sectionWallBottom + sectionWindowHeight, section.z - halfD,
        section.x - halfW, sectionWallBottom + sectionWindowHeight, section.z + halfD,
        section.x - halfW, section.wallHeight, section.z + halfD,
        section.x - halfW, section.wallHeight, section.z - halfD);

    // ---- RIGHT WALL (full wall with window) ----
    // Bottom solid part
    DrawTexturedQuad(section.x + halfW, 0.0f, section.z - halfD,
        section.x + halfW, 0.0f, section.z + halfD,
        section.x + halfW, sectionWallBottom, section.z + halfD,
        section.x + halfW, sectionWallBottom, section.z - halfD);
    // Top solid part
    DrawTexturedQuad(section.x + halfW, sectionWallBottom + sectionWindowHeight, section.z - halfD,
        section.x + halfW, sectionWallBottom + sectionWindowHeight, section.z + halfD,
        section.x + halfW, section.wallHeight, section.z + halfD,
        section.x + halfW, section.wallHeight, section.z - halfD);

    // ---- FRONT WALL (Now dynamic: containing the door opening) ----
    // Solid parts around the door
    DrawTexturedQuad(section.x - halfW, 0.0f, doorZ,
        section.x - halfW + doorWidth / 2, 0.0f, doorZ,
        section.x - halfW + doorWidth / 2, sectionWallBottom, doorZ,
        section.x - halfW, sectionWallBottom, doorZ);

    DrawTexturedQuad(section.x + halfW - doorWidth / 2, 0.0f, doorZ,
        section.x + halfW, 0.0f, doorZ,
        section.x + halfW, sectionWallBottom, doorZ,
        section.x + halfW - doorWidth / 2, sectionWallBottom, doorZ);

    // Top parts and lintel
    DrawTexturedQuad(section.x - halfW, sectionWallBottom + sectionWindowHeight, doorZ,
        section.x + halfW, sectionWallBottom + sectionWindowHeight, doorZ,
        section.x + halfW, section.wallHeight, doorZ,
        section.x - halfW, section.wallHeight, doorZ);

    float cornerX = section.x - (section.width * 0.35f);
    float cornerZ = section.z - (section.depth * 0.35f);

    drawSectionFurniture(section);

    // ===== DRAW TRANSPARENT WINDOWS =====
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setGlassMaterial();

    // Use pre-loaded glass texture
    glBindTexture(GL_TEXTURE_2D, glassTexture);

    // Window on the wall opposite the door
    DrawTexturedQuad(section.x - halfW, sectionWallBottom, backZ,
        section.x + halfW, sectionWallBottom, backZ,
        section.x + halfW, section.wallHeight, backZ,
        section.x - halfW, section.wallHeight, backZ);

    // Left wall window
    DrawTexturedQuad(section.x - halfW, sectionWallBottom, section.z - halfD,
        section.x - halfW, sectionWallBottom, section.z + halfD,
        section.x - halfW, section.wallHeight, section.z + halfD,
        section.x - halfW, section.wallHeight, section.z - halfD);

    // Right wall window
    DrawTexturedQuad(section.x + halfW, sectionWallBottom, section.z - halfD,
        section.x + halfW, sectionWallBottom, section.z + halfD,
        section.x + halfW, section.wallHeight, section.z + halfD,
        section.x + halfW, section.wallHeight, section.z - halfD);

    // Windows on the wall with the door (left and right of opening)
    DrawTexturedQuad(section.x - halfW, sectionWallBottom, doorZ,
        section.x - halfW + doorWidth / 2, sectionWallBottom, doorZ,
        section.x - halfW + doorWidth / 2, section.wallHeight, doorZ,
        section.x - halfW, section.wallHeight, doorZ);

    DrawTexturedQuad(section.x + halfW - doorWidth / 2, sectionWallBottom, doorZ,
        section.x + halfW, sectionWallBottom, doorZ,
        section.x + halfW, section.wallHeight, doorZ,
        section.x + halfW - doorWidth / 2, section.wallHeight, doorZ);

    glDisable(GL_NORMALIZE);
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
    drawMainShowroom();      // Draw main showroom first
    drawMainShowroomFurniture(); // Add furniture to main showroom
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
    glNormal3f(0.0f, 1.0f, 0.0f);  // Facing up
    glVertex3f(x - halfW, height, z - halfD);
    glVertex3f(x + halfW, height, z - halfD);
    glVertex3f(x + halfW, height, z + halfD);
    glVertex3f(x - halfW, height, z + halfD);
    glEnd();

    // Sides of platform
    glColor3f(0.7f, 0.7f, 0.7f);

    // Front side
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);  // Facing forward (negative Z)
    glVertex3f(x - halfW, 0.0f, z - halfD);
    glVertex3f(x + halfW, 0.0f, z - halfD);
    glVertex3f(x + halfW, height, z - halfD);
    glVertex3f(x - halfW, height, z - halfD);
    glEnd();

    // Back side
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // Facing backward (positive Z)
    glVertex3f(x - halfW, 0.0f, z + halfD);
    glVertex3f(x + halfW, 0.0f, z + halfD);
    glVertex3f(x + halfW, height, z + halfD);
    glVertex3f(x - halfW, height, z + halfD);
    glEnd();

    // Left side
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);  // Facing left (negative X)
    glVertex3f(x - halfW, 0.0f, z - halfD);
    glVertex3f(x - halfW, 0.0f, z + halfD);
    glVertex3f(x - halfW, height, z + halfD);
    glVertex3f(x - halfW, height, z - halfD);
    glEnd();

    // Right side
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);  // Facing right (positive X)
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
    float lightAmbient[] = { 0.3f, 0.1f, 0.1f, 1.0f };
    float lightDiffuse[] = { 0.6f, 0.3f, 0.3f, 1.0f };  // Reduced from 0.8f
    float lightSpecular[] = { 0.3f, 0.3f, 0.8f, 1.0f };  // Reduced from 1.0f

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Additional lights for each section - REDUCED intensity
    for (size_t i = 0; i < sections.size(); i++) {
        if (i < 4) {  // Reduced from 7 to 4 lights only
            GLenum lightID = GL_LIGHT1 + i;
            glEnable(lightID);

            float sectionLightPos[] = {
                sections[i].lightPosX,
                sections[i].lightPosY,
                sections[i].lightPosZ,
                1.0f
            };

            float sectionLightDiffuse[] = { 0.4f, 0.4f, 0.3f, 1.0f };  // Reduced intensity

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

void ShowRoomHelper::drawMainShowroomFurniture() {
    float halfW = mainWidth / 2.0f;
    float halfD = mainDepth / 2.0f;


    // Two sofas in waiting area
    FurnitureHelper::drawSofa(-halfW * 0.8f, 0.0f, halfD * 0.0f, 90.0f, 2.5f);
    FurnitureHelper::drawSofa(halfW * 0.8f, 0.0f, halfD * 0.0f, -90.0f, 2.5f);

    // Coffee tables between sofas
    FurnitureHelper::drawTable(0.0f, 0.0f, - halfD * 0.6f, 0.0f, 2.0f);

    // Information kiosks near entrance
    FurnitureHelper::drawInfoKiosk(-halfW * 0.15f, 0.0f, - halfD * 0.15f, 0.0f, 2.7f);
    FurnitureHelper::drawInfoKiosk(halfW * 0.15f, 0.0f, - halfD * 0.15f, 0.0f, 2.7f);


    // Brochure tables along side walls
    FurnitureHelper::drawBrochureTable(-halfW * 0.65f, 0.0f, 0.0f, 90.0f, 3.7f);
    FurnitureHelper::drawBrochureTable(halfW * 0.65f, 0.0f, 0.0f, 90.0f, 3.7f);

    // Modern chairs in waiting area
    float chairPositions[4][2] = {
        {-halfW * 0.05f, halfD * - 0.7f},
        {halfW * 0.05f, halfD * - 0.7f},
        {-halfW * 0.15f, halfD * - 0.6f},
        {halfW * 0.15f, halfD * - 0.6f}
    };
    for (int i = 0; i < 4; i++) {
        FurnitureHelper::drawModernChair(chairPositions[i][0], 0.0f, chairPositions[i][1],
            (i % 2 == 0) ? 45.0f : -45.0f, 3.6f);
    }

    // Flower pots near entrance
    FurnitureHelper::drawFlowerPot(-halfW * 0.15f, 0.0f, halfD * -0.5f, 0.0f, 2.5f);
    FurnitureHelper::drawFlowerPot(halfW * 0.15f, 0.0f, halfD * -0.5f, 0.0f, 2.5f);
    FurnitureHelper::drawFlowerPot(-halfW * 0.15f, 0.0f, halfD * 0.3f, 0.0f, 2.5f);
    FurnitureHelper::drawFlowerPot(halfW * 0.15f, 0.0f, halfD * 0.3f, 0.0f, 2.5f);
    FurnitureHelper::drawFlowerPot(-halfW * 0.15f, 0.0f, halfD * 0.6f, 0.0f, 2.5f);
    FurnitureHelper::drawFlowerPot(halfW * 0.15f, 0.0f, halfD * 0.6f, 0.0f, 2.5f);
}

void ShowRoomHelper::drawSectionFurniture(const ShowroomSection& section) {
    float halfW = section.width / 2.0f;
    float halfD = section.depth / 2.0f;

    // Different furniture layouts for different sections
    int sectionIndex = 0;
    for (size_t i = 0; i < sections.size(); i++) {
        if (&sections[i] == &section) {
            sectionIndex = i;
            break;
        }
    }

    switch (sectionIndex % 4) {
    case 0: // Luxury section
        FurnitureHelper::drawLuxuryConvertible(section.x, 0.3f, section.z, 0.0f, 4.0f);
        FurnitureHelper::drawDisplayStand(section.x - halfW * 0.8f, 0.0f,
            section.z + halfD * 0.8f, 45.0f, 3.0f);
        FurnitureHelper::drawModernChair(section.x + halfW * 0.8f, 0.0f,
            section.z - halfD * 0.8f, -30.0f, 3.5f);
        FurnitureHelper::drawBrochureTable(section.x, 0.0f, section.z - halfD * 0.9f,
            0.0f, 3.5f);
        FurnitureHelper::drawFlowerPot(section.x + halfW * 0.8f, 0.0f,
            section.z + halfD * 0.8f, 0.0f, 2.4f);
        break;

    case 1: // Sports section
        FurnitureHelper::drawSportsCar(section.x, 0.3f, section.z, 0.0f, 4.0f);
        FurnitureHelper::drawDisplayStand(section.x, 0.0f, section.z - halfW * 0.8f, 0.0f, 3.7f);
        FurnitureHelper::drawModernChair(section.x - halfW * 0.8f, 0.0f,
            section.z + halfD * 0.8f, 90.0f, 3.5f);
        FurnitureHelper::drawModernChair(section.x + halfW * 0.8f, 0.0f,
            section.z + halfD * 0.8f, -90.0f, 3.5f);
        FurnitureHelper::drawFlowerPot(section.x - halfW * 0.8f, 0.0f, section.z - halfD * 0.8f,
            0.0f, 3.5f);
        break;

    case 2: // Family section
        FurnitureHelper::drawFamilySUV(section.x, 0.3f, section.z, 45.0f, 4.0f);
        FurnitureHelper::drawSofa(section.x + halfW * 0.7f, 0.0f, section.z - halfD * 0.7f,
            -60.0f, 2.2f);
        FurnitureHelper::drawTable(section.x + halfW * 0.8f, 0.0f, section.z + halfD * 0.8f,
            0.0f, 1.6f);
        FurnitureHelper::drawFlowerPot(section.x - halfW * 0.8f, 0.0f,
            section.z + halfD * 0.8f, 0.0f, 3.5f);
        break;

    case 3: // Executive section
        FurnitureHelper::drawMiniRaceCar(section.x, 0.3f, section.z, 45.0f, 4.0f);
        
        FurnitureHelper::drawDisplayStand(section.x - halfW * 0.8f, 0.0f,
            section.z, 0.0f, 3.6f);
        FurnitureHelper::drawDisplayStand(section.x + halfW * 0.8f, 0.0f,
            section.z, 0.0f, 3.6f);
        FurnitureHelper::drawModernChair(section.x, 0.0f, section.z + halfD * 0.8f,
            180.0f, 3.6f);
        break;
    }
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