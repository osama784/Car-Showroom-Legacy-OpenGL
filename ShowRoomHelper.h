#ifndef ShowRoomHelper_H
#define ShowRoomHelper_H

#include <windows.h>
#include <gl.h>
#include <glu.h>
#include "stb_image.h"

class ShowRoomHelper {
public:
    // Constructor/Destructor
    ShowRoomHelper();
    ~ShowRoomHelper();

    void initialize();
    void loadDefaultTextures();

    // Basic drawing functions
    static void DrawTexturedQuad(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        float x4, float y4, float z4);

    // Room drawing functions
    void DrawRoom(float roomWidth, float roomHeight, float roomDepth);

    // Text rendering (if you want to add labels)
    static void RenderText(float x, float y, const char* text);


private:
    // Helper functions
    static void EnableTexturing(bool enable);
    static void setGlassMaterial();
    static void resetMaterial();

    GLuint wallTexture, floorTexture, ceilingTexture;
};

#endif // ShowRoomHelper_H