#ifndef SHOWROOMHELPER_H
#define SHOWROOMHELPER_H

#include <windows.h>
#include <gl.h>
#include <glu.h>
#include <vector>
#include <string>

// Structure for individual showroom sections (garages)
struct ShowroomSection {
    float x, z;              // Position within main showroom
    float width, depth;      // Dimensions of this section
    float wallHeight;        // Height of dividing walls (shorter than main walls)
    GLuint floorTexture;     // Floor texture for this section
    GLuint wallTexture;      // Wall texture for this section
    int carCapacity;         // How many cars this section can hold
    int textureIndex;

    // Display area (platform for car)
    float displayPlatformX, displayPlatformZ;
    float displayPlatformWidth, displayPlatformDepth;

    // Lighting (each section can have its own lights)
    float lightPosX, lightPosY, lightPosZ;

    ShowroomSection() : x(0), z(0), width(20.0f), depth(15.0f), wallHeight(6.0f),
        floorTexture(0), wallTexture(0), carCapacity(1),
        displayPlatformX(0), displayPlatformZ(0),
        displayPlatformWidth(8.0f), displayPlatformDepth(12.0f),
        lightPosX(0), lightPosY(10.0f), lightPosZ(0) {
    }
};

class ShowRoomHelper {
private:
    // Main showroom textures
    GLuint mainFloorTexture;
    GLuint mainWallTexture;
    GLuint mainCeilingTexture;

    // Internal section textures
    GLuint sectionFloorTexture;
    GLuint sectionWallTexture;

    // Main showroom dimensions
    float mainWidth;
    float mainHeight;
    float mainDepth;

    // Internal sections/rooms
    std::vector<ShowroomSection> sections;
    std::vector<GLuint> sectionFloorTextures;
    GLuint glassTexture;  // Add this for the glass texture


public:
    // Constructor & Destructor
    ShowRoomHelper();
    ~ShowRoomHelper();

    // Initialization
    void initialize(float width = 100.0f, float height = 20.0f, float depth = 80.0f);
    void loadDefaultTextures();
    void setMainShowroomSize(float width, float height, float depth);

    // Section management
    void addSection(float x, float z, float width, float depth,
        float wallHeight = 6.0f, int carCapacity = 1,int textureIndex = 0);
    void createGridSections(int rows, int cols, float sectionWidth = 20.0f,
        float sectionDepth = 15.0f);
    void clearSections();


    // Drawing functions
    void drawMainShowroom();
    void drawSection(const ShowroomSection& section);
    void drawAllSections();
    void drawShowroomComplete();  // Draws everything


    void drawMainShowroomFurniture();
    void drawSectionFurniture(const ShowroomSection& section);

    // Display platforms (where cars will sit)
    void drawDisplayPlatform(float x, float z, float width, float depth, float height = 0.1f);
    void drawAllDisplayPlatforms();

    // Lighting
    void setupShowroomLighting();

    // Texturing helpers
    void DrawTexturedQuad(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        float x4, float y4, float z4);
    void drawTexturedCube(float x, float y, float z,
        float width, float height, float depth,
        GLuint textureID, float texScale = 1.0f);

    // Material functions
    void setGlassMaterial();
    void setFloorMaterial();
    void resetMaterial();
    void setSolidWallMaterial();

    // Getters
    float getMainWidth() const { return mainWidth; }
    float getMainHeight() const { return mainHeight; }
    float getMainDepth() const { return mainDepth; }
    const std::vector<ShowroomSection>& getSections() const { return sections; }
    size_t getSectionCount() const { return sections.size(); }

    // Utility
    void renderSectionInfo();  // For debugging
};

#endif // SHOWROOMHELPER_H