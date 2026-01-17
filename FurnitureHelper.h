#ifndef FURNITUREHELPER_H
#define FURNITUREHELPER_H

#include <windows.h>
#include <gl.h>
#include <glu.h>

class FurnitureHelper {
private:
    static GLuint woodTexture;
    static GLuint potTexture;
    static GLuint leafTexture;
    static GLuint leatherTexture;  // For sofas/chairs
    static GLuint metalTexture;    // For metal furniture
    static GLuint yellowMetalTexture;  // For car body
    static GLuint redMetalTexture;     // For car details
    static GLuint glassTexture;        // For car windows
    static GLuint tireTexture;         // For car tires
    static GLuint chromeTexture;       // For shiny metal parts

    static void drawBox(float width, float height, float depth, GLuint textureID);
    static void drawCylinder(float radius, float height, int slices, GLuint textureID, float angle);
    static void drawWheel(float x, float y, float z, float radius, float width, float angle);

public:
    static void initialize();

    // Basic furniture
    static void drawTable(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawFlowerPot(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);

    // Car showroom specific furniture
    static void drawReceptionDesk(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawSofa(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawDisplayStand(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawBrochureTable(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawLightFixture(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawModernChair(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawInfoKiosk(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    
    static void drawSportsCar(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawLuxuryConvertible(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawFamilySUV(float x, float y, float z, float rotation = 0.0f, float scale = 1.0f);
    static void drawMiniRaceCar(float x, float y, float z, float rotation, float scale);


};

#endif // FURNITUREHELPER_H