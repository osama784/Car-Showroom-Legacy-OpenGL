#ifndef GRAPHICSHELPER_H
#define GRAPHICSHELPER_H

#include <windows.h>
#include <gl.h>
#include <glu.h>
#include "stb_image.h"

class GraphicsHelper {
public:
    // Constructor/Destructor
    GraphicsHelper();
    ~GraphicsHelper();

    // Texture loading
    static GLuint LoadTexture(const char* filename);

    // Basic drawing functions
    static void DrawTexturedQuad(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        float x4, float y4, float z4);

    static void DrawColoredQuad(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        float x4, float y4, float z4,
        float r, float g, float b);

    static void DrawCube(float x, float y, float z,
        float width, float height, float depth,
        GLuint textureID = 0);

    static void DrawWall(float x, float y, float z,
        float width, float height, bool frontBack,
        GLuint textureID = 0);

    static void DrawGrid(float size, int divisions, float y = 0.0f);

    static void DrawAxis(float length = 5.0f);

    // Room drawing functions
    static void DrawRoom(float roomWidth, float roomHeight, float roomDepth,
        GLuint wallTexture, GLuint floorTexture, GLuint ceilingTexture);

    static void DrawWireframeRoom(float roomWidth, float roomHeight, float roomDepth);

    // Text rendering (if you want to add labels)
    static void RenderText(float x, float y, const char* text);

    // Utility functions
    static void SetColor(float r, float g, float b, float a = 1.0f);
    static void ResetColor();

private:
    // Helper functions
    static void EnableTexturing(bool enable);
};

#endif // GRAPHICSHELPER_H