#include "WorldHelper.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "TextureHelper.h"

// For texture loading
#define STB_IMAGE_IMPLEMENTATION


WorldHelper::WorldHelper()
    : groundSize(200.0f),
    skyboxSize(500.0f),
    worldScale(1.0f),
    timeOfDay(12.0f),
    showroomWidth(),
    showroomHight(),
    showroomDepth(){

    // Initialize textures to 0
    grassTexture = 0;
    roadTexture = 0;
    skyTexture = 0;
    treeTexture = 0;

    // Default light position (noon sun)
    lightPosition[0] = 50.0f;
    lightPosition[1] = 100.0f;
    lightPosition[2] = 50.0f;
    lightPosition[3] = 1.0f;

    // Default light color
    lightDiffuse[0] = 0.8f;
    lightDiffuse[1] = 0.8f;
    lightDiffuse[2] = 0.8f;
    lightDiffuse[3] = 1.0f;

    // Default clear color (sky blue)
    clearColor[0] = 0.53f;
    clearColor[1] = 0.81f;
    clearColor[2] = 0.98f;
    clearColor[3] = 1.0f;
}

WorldHelper::~WorldHelper() {
    // Cleanup textures if needed
}

void WorldHelper::initialize(float groundSize, float skyboxSize,float showroomWidth, float showroomHeight, float showroomDepth) {
    this->groundSize = groundSize;
    this->skyboxSize = skyboxSize;
    this->showroomWidth = showroomWidth;
    this->showroomHight = showroomHight;
    this->showroomDepth = showroomDepth;

    // Load textures
    loadDefaultTextures();

    // Generate world objects
    generateTrees(30);      // Create 30 trees
    generateBuildings(5);   // Create 5 buildings

    // Calculate initial lighting
    calculateLighting();
}

void WorldHelper::loadDefaultTextures() {
    // Try to load actual texture files
    grassTexture = TextureHelper::LoadTexture("grass.jpg");
    //roadTexture = TextureHelper::LoadTexture("road.png");
    skyTexture = TextureHelper::LoadTexture("sky.jpg");
    //treeTexture = TextureHelper::LoadTexture("tree.png");

}

void WorldHelper::drawGround() {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, grassTexture);

    float halfGround = groundSize / 2.0f;
    float texRepeat = groundSize / 10.0f;  // Scale texture appropriately

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfGround, 0.0f, -halfGround);
    glTexCoord2f(texRepeat, 0.0f); glVertex3f(halfGround, 0.0f, -halfGround);
    glTexCoord2f(texRepeat, texRepeat); glVertex3f(halfGround, 0.0f, halfGround);
    glTexCoord2f(0.0f, texRepeat); glVertex3f(-halfGround, 0.0f, halfGround);
    glEnd();
}

void WorldHelper::drawRoad() {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // If we have a road texture, use it
    if (roadTexture != 0) {
        glBindTexture(GL_TEXTURE_2D, roadTexture);
    }
    else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.3f, 0.3f, 0.3f);  // Gray color for road
    }

    float roadWidth = 8.0f;
    float halfGround = groundSize / 2.0f;
    float halfShowRoomDepth = showroomDepth / 2.0f;
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, halfShowRoomDepth);
    glBegin(GL_QUADS);
    // Road from far to near
    glVertex3f(-roadWidth / 2, 0.01f, 0.0f);
    glVertex3f(roadWidth / 2, 0.01f, 0.0f);
    glVertex3f(roadWidth / 2, 0.01f, halfGround - halfShowRoomDepth);
    glVertex3f(-roadWidth / 2, 0.01f, halfGround - halfShowRoomDepth);
    glEnd();
    
    // Draw road markings
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow lines
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    for (float z = 0; z < halfGround - 5.0f - halfShowRoomDepth; z += 10.0f) {
        glVertex3f(0.0f, 0.02f, z );
        glVertex3f(0.0f, 0.02f, z + 5.0f);
    }
    glEnd();
    glPopMatrix();

    if (roadTexture == 0) {
        glEnable(GL_TEXTURE_2D);
    }
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


void WorldHelper::drawSky() {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    float halfSize = skyboxSize / 2.0f;

    glBindTexture(GL_TEXTURE_2D, skyTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Draw sky as a dome (simplified - just a large quad)
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, 0.0f, -halfSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSize, 0.0f, -halfSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSize, halfSize, -halfSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, halfSize, -halfSize);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void WorldHelper::drawTree(float x, float z, float trunkHeight, float trunkWidth, float foliageHeight) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // Draw trunk
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.4f, 0.2f, 0.1f);  // Brown

    // Simple trunk as a cube
    glBegin(GL_QUADS);
    // Front
    glVertex3f(-trunkWidth / 2, 0.0f, -trunkWidth / 2);
    glVertex3f(trunkWidth / 2, 0.0f, -trunkWidth / 2);
    glVertex3f(trunkWidth / 2, trunkHeight, -trunkWidth / 2);
    glVertex3f(-trunkWidth / 2, trunkHeight, -trunkWidth / 2);

    // Back
    glVertex3f(-trunkWidth / 2, 0.0f, trunkWidth / 2);
    glVertex3f(trunkWidth / 2, 0.0f, trunkWidth / 2);
    glVertex3f(trunkWidth / 2, trunkHeight, trunkWidth / 2);
    glVertex3f(-trunkWidth / 2, trunkHeight, trunkWidth / 2);

    // Left
    glVertex3f(-trunkWidth / 2, 0.0f, -trunkWidth / 2);
    glVertex3f(-trunkWidth / 2, 0.0f, trunkWidth / 2);
    glVertex3f(-trunkWidth / 2, trunkHeight, trunkWidth / 2);
    glVertex3f(-trunkWidth / 2, trunkHeight, -trunkWidth / 2);

    // Right
    glVertex3f(trunkWidth / 2, 0.0f, -trunkWidth / 2);
    glVertex3f(trunkWidth / 2, 0.0f, trunkWidth / 2);
    glVertex3f(trunkWidth / 2, trunkHeight, trunkWidth / 2);
    glVertex3f(trunkWidth / 2, trunkHeight, -trunkWidth / 2);
    glEnd();

    // Draw foliage (leaves)
    glColor3f(0.1f, 0.6f, 0.1f);  // Green
    glTranslatef(0.0f, trunkHeight, 0.0f);

    // Simple foliage as a sphere-ish shape (using multiple layers)
    float foliageRadius = trunkWidth * 3.0f;
    int layers = 3;

    for (int i = 0; i < layers; i++) {
        float y = i * foliageHeight / (float)layers / 1.5;
        float radius = foliageRadius * (1.0f - (float)i / (float)layers);
        float layerHeight = foliageHeight / (float)layers;

        // Top of layer
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, y, 0.0f);
        for (int j = 0; j <= 16; j++) {
            float angle = j * 2.0f * 3.14159f / 16.0f;
            glVertex3f(radius * cos(angle), y - layerHeight / 2, radius * sin(angle));
        }
        glEnd();

        // Bottom of layer
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, y - layerHeight, 0.0f);
        for (int j = 0; j <= 16; j++) {
            float angle = j * 2.0f * 3.14159f / 16.0f;
            glVertex3f(radius * cos(angle), y - layerHeight / 2, radius * sin(angle));
        }
        glEnd();
    }

    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void WorldHelper::drawBuilding(float x, float z, float width, float depth, float height, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, height / 2, z);

    glDisable(GL_TEXTURE_2D);
    glColor3f(r, g, b);

    // Draw main building cube
    glBegin(GL_QUADS);
    // Front face
    glVertex3f(-width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);
    glVertex3f(-width / 2, height / 2, -depth / 2);

    // Back face
    glVertex3f(-width / 2, -height / 2, depth / 2);
    glVertex3f(width / 2, -height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);

    // Left face
    glVertex3f(-width / 2, -height / 2, -depth / 2);
    glVertex3f(-width / 2, -height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, -depth / 2);

    // Right face
    glVertex3f(width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, -height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);

    // Top face
    glVertex3f(-width / 2, height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);
    glEnd();

    // Draw windows (optional)
    glColor3f(0.1f, 0.1f, 0.2f);  // Dark blue windows
    float windowWidth = width / 8.0f;
    float windowHeight = height / 10.0f;
    float windowSpacingX = width / 4.0f;
    float windowSpacingY = height / 5.0f;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            float wx = -width / 3 + col * windowSpacingX;
            float wy = -height / 3 + row * windowSpacingY;

            // Front windows
            glBegin(GL_QUADS);
            glVertex3f(wx - windowWidth / 2, wy - windowHeight / 2, -depth / 2 - 0.01f);
            glVertex3f(wx + windowWidth / 2, wy - windowHeight / 2, -depth / 2 - 0.01f);
            glVertex3f(wx + windowWidth / 2, wy + windowHeight / 2, -depth / 2 - 0.01f);
            glVertex3f(wx - windowWidth / 2, wy + windowHeight / 2, -depth / 2 - 0.01f);
            glEnd();
        }
    }

    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void WorldHelper::generateTrees(int count) {
    trees.clear();
    srand(time(NULL));

    float halfGround = groundSize / 2.0f;
    float minDistanceFromCenter = 70.0f;  // Don't place trees too close to center
    float minDistanceFromRoad = 10.0f;    // Don't place trees too close to road
    float safeMargin = 5.0f;              // Safe margin from ground edges

    // Define road area to avoid placing trees on the road
    float roadHalfWidth = 4.0f;           // Road width is 8.0f, so half is 4.0f
    float roadStartZ = 0.0f;              // Road starts from showroom
    float roadEndZ = halfGround;          // Road extends to ground edge

    for (int i = 0; i < count; i++) {
        Tree tree;
        bool validPosition = false;
        int attempts = 0;
        const int MAX_ATTEMPTS = 100;  // Prevent infinite loop

        // Keep trying until we find a valid position
        while (!validPosition && attempts < MAX_ATTEMPTS) {
            // Random position within ground boundaries with safe margin
            float maxPos = halfGround - safeMargin;
            tree.x = (rand() % (int)(maxPos * 2)) - maxPos;
            tree.z = (rand() % (int)(maxPos * 2)) - maxPos;

            // Check distance from center (showroom area)
            float distFromCenter = sqrt(tree.x * tree.x + tree.z * tree.z);

            // Check if too close to road
            bool tooCloseToRoad = false;
            if (tree.z >= roadStartZ && tree.z <= roadEndZ) {
                if (fabs(tree.x) < roadHalfWidth + minDistanceFromRoad) {
                    tooCloseToRoad = true;
                }
            }

            // Check if position is valid
            if (distFromCenter >= minDistanceFromCenter && !tooCloseToRoad) {
                // Check if this tree would overlap with existing trees
                bool overlaps = false;
                float minTreeDistance = 8.0f;  // Minimum distance between trees

                for (const auto& existingTree : trees) {
                    float dx = tree.x - existingTree.x;
                    float dz = tree.z - existingTree.z;
                    float distance = sqrt(dx * dx + dz * dz);

                    if (distance < minTreeDistance) {
                        overlaps = true;
                        break;
                    }
                }

                if (!overlaps) {
                    validPosition = true;
                }
            }

            attempts++;
        }

        // If we couldn't find a valid position after many attempts,
        // place the tree anyway but far from center
        if (!validPosition) {
            // Place at edge of ground with safe margin
            float angle = (rand() % 360) * 3.14159f / 180.0f;
            float distance = halfGround - safeMargin - 10.0f;  // 10 units from edge
            tree.x = cos(angle) * distance;
            tree.z = sin(angle) * distance;
        }

        // Random size
        tree.height = 5.0f + (rand() % 100) / 10.0f;       // 5-15 units tall
        tree.trunkWidth = 0.3f + (rand() % 50) / 100.0f;   // 0.3-0.8 units wide

        trees.push_back(tree);
    }
}

void WorldHelper::generateBuildings(int count) {
    buildings.clear();

    // Always create exactly 8 buildings in smart fixed positions
    const int targetCount = 8;

    std::cout << "=== Generating " << targetCount << " fixed buildings ===" << std::endl;

    // Smart fixed positions around the showroom
    // We'll use arrays instead of tuples to avoid compilation issues
    struct BuildingData {
        float x, z, width, depth, height, r, g, b;
    };

    BuildingData fixedBuildings[] = {
        // === BUILDINGS IN FRONT OF SHOWROOM (4 buildings) ===

        // 1. Front-right building
        {100.0f, 150.0f, 20.0f, 16.0f, 35.0f, 0.6f, 0.5f, 0.4f},

        // 2. Front-left building
        {-100.0f, 150.0f, 18.0f, 15.0f, 32.0f, 0.5f, 0.6f, 0.5f},

        // 3. Front-far-right
        {100.0f, 75.0f, 25.0f, 20.0f, 40.0f, 0.7f, 0.5f, 0.5f},

        // 4. Front-far-left
        {-100.0f, 75.0f, 22.0f, 18.0f, 38.0f, 0.5f, 0.5f, 0.7f},

        // === BUILDINGS BEHIND SHOWROOM (4 buildings) ===

        // 5. Back-right
        {120.0f, -75.0f, 16.0f, 14.0f, 28.0f, 0.6f, 0.6f, 0.4f},

        // 6. Back-left
        {-120.0f, -75.0f, 17.0f, 13.0f, 30.0f, 0.4f, 0.6f, 0.6f},

        // 7. Back-far-right
        {120.0f, -150.0f, 20.0f, 17.0f, 33.0f, 0.6f, 0.4f, 0.6f},

        // 8. Back-far-left
        {-120.0f, -150.0f, 19.0f, 16.0f, 31.0f, 0.5f, 0.7f, 0.5f}
    };

    // Calculate array size
    int buildingCount = sizeof(fixedBuildings) / sizeof(fixedBuildings[0]);

    // Create all fixed buildings
    for (int i = 0; i < buildingCount; i++) {
        Building building;

        // Copy building data
        building.x = fixedBuildings[i].x;
        building.z = fixedBuildings[i].z;
        building.width = fixedBuildings[i].width;
        building.depth = fixedBuildings[i].depth;
        building.height = fixedBuildings[i].height;
        building.r = fixedBuildings[i].r;
        building.g = fixedBuildings[i].g;
        building.b = fixedBuildings[i].b;

        // Add to buildings vector
        buildings.push_back(building);

        // Print debug information
        std::cout << "Building " << (i + 1) << " placed at: ("
            << building.x << ", " << building.z << ")" << std::endl;
        std::cout << "  Size: " << building.width << " x "
            << building.depth << " x " << building.height << std::endl;
        std::cout << "  Color: (" << building.r << ", "
            << building.g << ", " << building.b << ")" << std::endl;
        std::cout << "  Position: "
            << (building.z > 0 ? "Front" : "Back") << " - "
            << (building.x > 0 ? "Right" : "Left") << std::endl;
    }

    // Summary
    std::cout << "=== Buildings Generation Complete ===" << std::endl;
    std::cout << "Total buildings created: " << buildings.size() << std::endl;

    // Count buildings in front and back
    int frontCount = 0;
    int backCount = 0;
    for (int i = 0; i < buildings.size(); i++) {  // FIXED: size_t instead of int
        if (buildings[i].z > 0) {
            frontCount++;
        }
        else {
            backCount++;
        }
    }

    std::cout << "Buildings in front of showroom (z > 0): " << frontCount << std::endl;
    std::cout << "Buildings behind showroom (z < 0): " << backCount << std::endl;
}

void WorldHelper::drawOutsideWorld() {
    // Draw in correct order:
    // 1. Sky (background)
    drawSky();

    // 2. Ground (opaque)
    drawGround();

    // 3. Roads
    drawRoad();

    // 4. Trees and buildings
    for (const auto& tree : trees) {
        drawTree(tree.x, tree.z,
            tree.height * 0.3f,  // Trunk height
            tree.trunkWidth,
            tree.height * 0.7f); // Foliage height
    }

    for (const auto& building : buildings) {
        drawBuilding(building.x, building.z,
            building.width, building.depth, building.height,
            building.r, building.g, building.b);
    }
}

void WorldHelper::drawWorldWithLighting() {
    // Set up lighting for the current time of day
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float lightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Draw the world
    drawOutsideWorld();

    glDisable(GL_LIGHTING);
}

void WorldHelper::setTimeOfDay(float hour) {
    timeOfDay = hour;
    if (timeOfDay < 0.0f) timeOfDay = 0.0f;
    if (timeOfDay > 24.0f) timeOfDay = 24.0f;

    calculateLighting();
}

void WorldHelper::advanceTime(float deltaHours) {
    timeOfDay += deltaHours;
    if (timeOfDay >= 24.0f) timeOfDay -= 24.0f;
    if (timeOfDay < 0.0f) timeOfDay += 24.0f;

    calculateLighting();
}

void WorldHelper::calculateLighting() {
    // Calculate sun position based on time of day
    float sunAngle = (timeOfDay / 24.0f) * 360.0f;
    float sunHeight = sin(sunAngle * 3.14159f / 180.0f) * 100.0f;
    float sunDistance = 150.0f;

    // Sun position (moving in a circle)
    lightPosition[0] = cos(sunAngle * 3.14159f / 180.0f) * sunDistance;
    lightPosition[1] = sunHeight;
    lightPosition[2] = sin(sunAngle * 3.14159f / 180.0f) * sunDistance;

    // Adjust light color based on time
    if (timeOfDay > 6.0f && timeOfDay < 7.0f) {
        // Daytime
        lightDiffuse[0] = 0.8f;
        lightDiffuse[1] = 0.8f;
        lightDiffuse[2] = 0.8f;

        clearColor[0] = 0.53f;  // Sky blue
        clearColor[1] = 0.81f;
        clearColor[2] = 0.98f;
    }
    else if (timeOfDay > 17.0f && timeOfDay < 19.0f) {
        // Sunset
        lightDiffuse[0] = 0.9f;  // Orange tint
        lightDiffuse[1] = 0.6f;
        lightDiffuse[2] = 0.4f;

        clearColor[0] = 1.0f;    // Orange sky
        clearColor[1] = 0.6f;
        clearColor[2] = 0.2f;
    }
    else {
        // Nighttime
        lightDiffuse[0] = 0.1f;
        lightDiffuse[1] = 0.1f;
        lightDiffuse[2] = 0.2f;

        clearColor[0] = 0.05f;   // Dark blue
        clearColor[1] = 0.05f;
        clearColor[2] = 0.1f;
    }

    // Update OpenGL clear color
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
}

float WorldHelper::getTimeOfDay() const {
    return timeOfDay;
}

void WorldHelper::reset() {
    trees.clear();
    buildings.clear();
    generateTrees(30);
    generateBuildings(5);
    setTimeOfDay(12.0f);  // Reset to noon
}

void WorldHelper::renderWorldInfo(float x, float y) {
    // This is a placeholder for rendering text information about the world
    // In practice, you'd use a text rendering system
    std::cout << "World Info: Time = " << timeOfDay
        << ", Trees = " << trees.size()
        << ", Buildings = " << buildings.size() << std::endl;
}



bool WorldHelper::checkCameraCollision(float camX, float camZ, float camRadius) {

    // Check collision with trees (circular collision)
    for (const auto& tree : trees) {
        float dx = camX - tree.x;
        float dz = camZ - tree.z;
        float distance = sqrt(dx * dx + dz * dz);

        if (distance < camRadius + tree.radius()) {
            return true; // Collision with tree
        }
    }

    // Check collision with buildings (AABB collision)
    for (const auto& building : buildings) {
        if (building.collides(camX, camZ, camRadius)) {
            return true; // Collision with building
        }
    }

    return false; // No collision
}
