#ifndef WORLDHELPER_H
#define WORLDHELPER_H

#include <windows.h>
#include <gl.h>
#include <glu.h>
#include <vector>
#include <ctime>
#include "stb_image.h"

struct Tree {
    float x, z;
    float height;
    float trunkWidth;
};

struct Building {
    float x, z;
    float width, depth, height;
    float r, g, b;  // Building color
};

class WorldHelper {
private:
    // Textures
    GLuint grassTexture;
    GLuint roadTexture;
    GLuint skyTexture;
    GLuint treeTexture;

    // World dimensions
    float groundSize;
    float worldScale;

    // showroom demensions
    GLuint showroomWidth;
    GLuint showroomHight;
    GLuint showroomDepth;

    // Skybox size
    float skyboxSize;

    // Collections of world objects
    std::vector<Tree> trees;
    std::vector<Building> buildings;

    // Time of day
    float timeOfDay;
    float lightPosition[4];
    float lightDiffuse[4];
    float clearColor[4];

    // Helper functions
    void drawGround();
    void drawRoad();
    void drawSky();
    void drawTree(float x, float z, float trunkHeight, float trunkWidth, float foliageHeight);
    void drawBuilding(float x, float z, float width, float depth, float height, float r, float g, float b);
    void generateTrees(int count);
    void generateBuildings(int count);
    void calculateLighting();

public:
    // Constructor & Destructor
    WorldHelper();
    ~WorldHelper();

    // Initialization
    void initialize(float groundSize, float skyboxSize, float showroomWidth, float showroomHeight, float showroomDepth);
    void loadDefaultTextures();

    // Drawing functions
    void drawOutsideWorld();
    void drawWorldWithLighting();

    // Time control
    void setTimeOfDay(float hour);
    void advanceTime(float deltaHours);
    float getTimeOfDay() const;

    // Reset
    void reset();

    // Debug
    void renderWorldInfo(float x, float y);
};

#endif // WORLDHELPER_H