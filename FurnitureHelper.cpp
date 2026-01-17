#include "FurnitureHelper.h"
#include "TextureHelper.h"
#include <iostream>

GLuint FurnitureHelper::woodTexture = 0;
GLuint FurnitureHelper::potTexture = 0;
GLuint FurnitureHelper::leafTexture = 0;
GLuint FurnitureHelper::leatherTexture = 0;
GLuint FurnitureHelper::metalTexture = 0;
GLuint FurnitureHelper::yellowMetalTexture = 0;  // Add this
GLuint FurnitureHelper::redMetalTexture = 0;     // Add this
GLuint FurnitureHelper::glassTexture = 0;        // Add this
GLuint FurnitureHelper::tireTexture = 0;         // Add this
GLuint FurnitureHelper::chromeTexture = 0;

void FurnitureHelper::initialize() {
    woodTexture = TextureHelper::LoadTexture("floor.png");
    potTexture = TextureHelper::LoadTexture("marble.jpg");
    leafTexture = TextureHelper::LoadTexture("grass.jpg");
    leatherTexture = TextureHelper::LoadTexture("cobblestone.jpg");
    metalTexture = TextureHelper::LoadTexture("brick.jpg");

    // Load car-specific textures
    yellowMetalTexture = TextureHelper::LoadTexture("yellow.jpg");
    redMetalTexture = TextureHelper::LoadTexture("red.jpg");
    glassTexture = TextureHelper::LoadTexture("glass.jpg");
    tireTexture = TextureHelper::LoadTexture("brick.jpg");    // Using brick as tire texture
    chromeTexture = TextureHelper::LoadTexture("glass.jpg"); // Using marble as chrome

    std::cout << "Furniture textures loaded successfully!" << std::endl;
}

void FurnitureHelper::drawWheel(float x, float y, float z, float radius, float width, float angle) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // IMPORTANT: Rotate the wheel to face forward (along Z-axis)
    // This rotates the cylinder from vertical (Y-axis) to horizontal (X-axis)
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    // Tire (outer part)
    glPushMatrix();
    glColor4f(0.1f, 0.1f, 0.1f, 1.0f); // Black tire
    float mat_tire_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float mat_tire_shininess[] = { 0.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_tire_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_tire_shininess);
    drawCylinder(radius, width, 32, tireTexture, 0);
    glPopMatrix();

    // Rim (inner part)
    glPushMatrix();
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f); // Silver rim
    float mat_rim_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float mat_rim_shininess[] = { 80.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_rim_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_rim_shininess);
    drawCylinder(radius * 0.7f, width + 0.01f, 16, chromeTexture, 0);
    glPopMatrix();

    glPopMatrix();
}

void FurnitureHelper::drawSportsCar(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Set car body material (shiny yellow)
    glColor4f(1.0f, 1.0f, 0.8f, 1.0f); // Yellow tint
    float mat_body_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float mat_body_shininess[] = { 100.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_body_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_body_shininess);

    // MAIN CHASSIS (car body)
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, yellowMetalTexture);
    // Main body section
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    drawBox(1.8f, 0.5f, 4.5f, yellowMetalTexture);
    glPopMatrix();

    // Hood (front)
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 1.5f);
    drawBox(1.6f, 0.4f, 1.2f, yellowMetalTexture);
    glPopMatrix();

    // Trunk (rear)
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, -1.5f);
    drawBox(1.6f, 0.4f, 1.2f, yellowMetalTexture);
    glPopMatrix();
    glPopMatrix();

    // CABIN SECTION
    glPushMatrix();
    // Windshield area
    

    // Side windows and doors
    glBindTexture(GL_TEXTURE_2D, glassTexture);
    glColor4f(0.7f, 0.8f, 1.0f, 0.6f); // Tinted blue windows
    // Left window
    glPushMatrix();
    glTranslatef(-0.7f, 0.9f, 0.0f);
    drawBox(0.05f, 0.4f, 1.4f, glassTexture);
    glPopMatrix();

    // Right window
    glPushMatrix();
    glTranslatef(0.7f, 0.9f, 0.0f);
    drawBox(0.05f, 0.4f, 1.4f, glassTexture);
    glPopMatrix();

    // Front windshield
    glPushMatrix();
    glTranslatef(0.0f, 0.95f, 0.8f);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    drawBox(1.3f, 0.2f, 0.8f, glassTexture);
    glPopMatrix();

    // Rear windshield
    glPushMatrix();
    glTranslatef(0.0f, 0.95f, -0.8f);
    glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
    drawBox(1.3f, 0.2f, 0.8f, glassTexture);
    glPopMatrix();
    glPopMatrix();

    // RED DETAILS
    glBindTexture(GL_TEXTURE_2D, redMetalTexture);
    glColor4f(1.0f, 0.3f, 0.3f, 1.0f); // Red color

    // Front bumper
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 2.1f);
    drawBox(1.6f, 0.15f, 0.1f, redMetalTexture);
    glPopMatrix();

    // Rear bumper
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, -2.1f);
    drawBox(1.6f, 0.15f, 0.1f, redMetalTexture);
    glPopMatrix();

    // Side skirts
    glPushMatrix();
    glTranslatef(-0.7f, 0.25f, 0.0f);
    drawBox(0.05f, 0.1f, 2.5f, redMetalTexture);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7f, 0.25f, 0.0f);
    drawBox(0.05f, 0.1f, 2.5f, redMetalTexture);
    glPopMatrix();

    // Spoiler
    glPushMatrix();
    glTranslatef(0.0f, 1.1f, -2.0f);
    drawBox(1.2f, 0.1f, 0.4f, redMetalTexture);
    glPopMatrix();

    // Spoiler supports
    glPushMatrix();
    glTranslatef(-0.5f, 0.8f, -1.9f);
    drawBox(0.05f, 0.3f, 0.05f, redMetalTexture);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 0.8f, -1.9f);
    drawBox(0.05f, 0.3f, 0.05f, redMetalTexture);
    glPopMatrix();

    // CHROME DETAILS
    glBindTexture(GL_TEXTURE_2D, chromeTexture);
    glColor4f(0.9f, 0.9f, 0.9f, 1.0f); // Chrome color
    float mat_chrome_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float mat_chrome_shininess[] = { 120.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_chrome_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_chrome_shininess);

    // Front grill
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 2.05f);
    drawBox(1.0f, 0.3f, 0.05f, chromeTexture);
    glPopMatrix();

    // Headlights
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.6f, 0.6f, 2.05f);
        drawBox(0.2f, 0.15f, 0.05f, chromeTexture);
        glPopMatrix();
    }

    // Taillights
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.6f, 0.6f, -2.05f);
        drawBox(0.2f, 0.15f, 0.05f, chromeTexture);
        glPopMatrix();
    }

    // Side mirrors
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.95f, 1.0f, 0.7f);
        drawBox(0.1f, 0.15f, 0.05f, chromeTexture);
        glPopMatrix();
    }

    // Exhaust pipes
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.3f, 0.3f, -2.15f);
        drawBox(0.08f, 0.08f, 0.15f, chromeTexture);
        glPopMatrix();
    }

    // WHEELS (positioned at the corners of the car)
    // Front left wheel
    drawWheel(-0.75f, 0.3f, 1.5f, 0.35f, 0.2f, -90.0f);

    // Front right wheel
    drawWheel(0.75f, 0.3f, 1.5f, 0.35f, 0.2f, +90.0f);

    // Rear left wheel
    drawWheel(-0.75f, 0.3f, -1.5f, 0.35f, 0.2f, -90.0f);

    // Rear right wheel
    drawWheel(0.75f, 0.3f, -1.5f, 0.35f, 0.2f, +90.0f);

    // Interior details
    glBindTexture(GL_TEXTURE_2D, leatherTexture);
    glColor4f(0.2f, 0.2f, 0.2f, 1.0f); // Dark leather

    // Seats
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.4f, 0.7f, 0.0f);
        drawBox(0.3f, 0.25f, 0.4f, leatherTexture);
        glPopMatrix();
    }

    // Dashboard
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 1.0f);
    drawBox(1.2f, 0.15f, 0.3f, leatherTexture);
    glPopMatrix();

    // Steering wheel
    glPushMatrix();
    glTranslatef(-0.3f, 0.8f, 0.9f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    drawBox(0.25f, 0.05f, 0.05f, chromeTexture);
    glPopMatrix();

    glPopMatrix(); // End car
}



void FurnitureHelper::drawCylinder(float radius, float height, int slices, GLuint textureID, float angle) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);


    // Draw cylinder body (along Z-axis)
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * 3.14159f * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        // Calculate normal (points outward from center)
        float nx = cos(angle);
        float ny = sin(angle);

        glNormal3f(nx, ny, 0.0f);
        glTexCoord2f((float)i / slices, 0.0f);
        glVertex3f(x, y, 0.0f);
        glTexCoord2f((float)i / slices, 1.0f);
        glVertex3f(x, y, height);
    }
    glEnd();

    // Draw top cap (front face)
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f); // Facing forward
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, height);
    for (int i = 0; i <= slices; i++) {
        float angle = 2.0f * 3.14159f * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glTexCoord2f(0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle));
        glVertex3f(x, y, height);
    }
    glEnd();

    // Draw bottom cap (back face)
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f); // Facing backward
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = slices; i >= 0; i--) {
        float angle = 2.0f * 3.14159f * i / slices;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glTexCoord2f(0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle));
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}

void FurnitureHelper::drawBox(float width, float height, float depth, GLuint textureID) {
    float w = width / 2.0f;
    float d = depth / 2.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  // Opaque white
    float mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float mat_shininess[] = { 20.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_QUADS);
    // Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-w, 0, d);
    glTexCoord2f(1, 0); glVertex3f(w, 0, d);
    glTexCoord2f(1, 1); glVertex3f(w, height, d);
    glTexCoord2f(0, 1); glVertex3f(-w, height, d);
    // Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(-w, 0, -d);
    glTexCoord2f(1, 0); glVertex3f(w, 0, -d);
    glTexCoord2f(1, 1); glVertex3f(w, height, -d);
    glTexCoord2f(0, 1); glVertex3f(-w, height, -d);
    // Top
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-w, height, -d);
    glTexCoord2f(1, 0); glVertex3f(w, height, -d);
    glTexCoord2f(1, 1); glVertex3f(w, height, d);
    glTexCoord2f(0, 1); glVertex3f(-w, height, d);
    // Left side
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-w, 0, -d);
    glTexCoord2f(1, 0); glVertex3f(-w, 0, d);
    glTexCoord2f(1, 1); glVertex3f(-w, height, d);
    glTexCoord2f(0, 1); glVertex3f(-w, height, -d);
    // Right side
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(w, 0, -d);
    glTexCoord2f(1, 0); glVertex3f(w, 0, d);
    glTexCoord2f(1, 1); glVertex3f(w, height, d);
    glTexCoord2f(0, 1); glVertex3f(w, height, -d);
    glEnd();
}

void FurnitureHelper::drawTable(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Table Top (4.0 x 0.2 x 2.5)
    glPushMatrix();
    glTranslatef(0, 1.5f, 0);
    drawBox(4.0f, 0.2f, 2.5f, woodTexture);
    glPopMatrix();

    // 4 Legs
    float legX = 1.8f, legZ = 1.1f;
    for (int i = 0; i < 4; i++) {
        float tx = (i < 2) ? legX : -legX;
        float tz = (i % 2 == 0) ? legZ : -legZ;
        glPushMatrix();
        glTranslatef(tx, 0, tz);
        drawBox(0.2f, 1.5f, 0.2f, woodTexture);
        glPopMatrix();
    }
    glPopMatrix();
}

void FurnitureHelper::drawFlowerPot(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // The Pot
    drawBox(0.8f, 1.0f, 0.8f, potTexture);

    // The Plant (Simplified box style)
    glPushMatrix();
    glTranslatef(0, 1.0f, 0);
    drawBox(0.6f, 1.2f, 0.6f, leafTexture);
    glPopMatrix();

    glPopMatrix();
}

// NEW FURNITURE FUNCTIONS:

void FurnitureHelper::drawReceptionDesk(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Main counter top (6.0 x 0.2 x 2.0)
    glPushMatrix();
    glTranslatef(0, 1.0f, 0);
    drawBox(6.0f, 0.2f, 2.0f, woodTexture);
    glPopMatrix();

    // Front panel (6.0 x 1.0 x 0.1)
    glPushMatrix();
    glTranslatef(0, 0.5f, 0.95f);
    drawBox(6.0f, 1.0f, 0.1f, woodTexture);
    glPopMatrix();

    // Back panel (6.0 x 1.2 x 0.1)
    glPushMatrix();
    glTranslatef(0, 0.6f, -0.95f);
    drawBox(6.0f, 1.2f, 0.1f, woodTexture);
    glPopMatrix();

    // Side panels (3 legs on each side)
    for (int i = -1; i <= 1; i++) {
        float posX = i * 1.8f;
        // Left side legs
        glPushMatrix();
        glTranslatef(posX - 0.4f, 0.5f, 0.6f);
        drawBox(0.2f, 1.0f, 0.2f, woodTexture);
        glPopMatrix();

        // Right side legs
        glPushMatrix();
        glTranslatef(posX - 0.4f, 0.5f, -0.6f);
        drawBox(0.2f, 1.0f, 0.2f, woodTexture);
        glPopMatrix();
    }

    glPopMatrix();
}

void FurnitureHelper::drawSofa(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Seat (3.0 x 0.8 x 1.0)
    glPushMatrix();
    glTranslatef(0, 0.4f, 0);
    drawBox(3.0f, 0.8f, 1.0f, leatherTexture);
    glPopMatrix();

    // Back (3.0 x 1.2 x 0.2)
    glPushMatrix();
    glTranslatef(0, 1.2f, -0.4f);
    drawBox(3.0f, 1.2f, 0.2f, leatherTexture);
    glPopMatrix();

    // Arms (2 arms)
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 1.4f, 0.8f, 0);
        drawBox(0.2f, 1.6f, 1.0f, leatherTexture);
        glPopMatrix();
    }

    // Legs (4 legs)
    float legPositions[4][2] = { {-1.4f, -0.4f}, {1.4f, -0.4f}, {-1.4f, 0.4f}, {1.4f, 0.4f} };
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legPositions[i][0], 0.2f, legPositions[i][1]);
        drawBox(0.15f, 0.4f, 0.15f, woodTexture);
        glPopMatrix();
    }

    glPopMatrix();
}

void FurnitureHelper::drawDisplayStand(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Top display platform (1.5 x 0.1 x 1.5)
    glPushMatrix();
    glTranslatef(0, 2.0f, 0);
    drawBox(1.5f, 0.1f, 1.5f, metalTexture);
    glPopMatrix();

    // Middle platform (1.2 x 0.1 x 1.2)
    glPushMatrix();
    glTranslatef(0, 1.2f, 0);
    drawBox(1.2f, 0.1f, 1.2f, metalTexture);
    glPopMatrix();

    // Central column
    glPushMatrix();
    glTranslatef(0, 0.2f, 0);
    drawBox(0.2f, 2.0f, 0.2f, metalTexture);
    glPopMatrix();

    // Base (1.0 x 0.2 x 1.0)
    glPushMatrix();
    drawBox(1.0f, 0.2f, 1.0f, metalTexture);
    glPopMatrix();

    glPopMatrix();
}
void FurnitureHelper::drawBrochureTable(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Central column (0.3 x 1.0 x 0.3)
    glPushMatrix();
    glTranslatef(0, 0.0f, 0);
    drawBox(0.3f, 1.0f, 0.3f, woodTexture);
    glPopMatrix();

    // Table top (1.6 x 0.08 x 1.0)
    glPushMatrix();
    glTranslatef(0, 1.0f + 0.04f, 0);
    drawBox(1.6f, 0.08f, 1.0f, woodTexture);
    glPopMatrix();


    glPopMatrix();
}
void FurnitureHelper::drawModernChair(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Seat (0.8 x 0.1 x 0.8)
    glPushMatrix();
    glTranslatef(0, 0.5f, 0);
    drawBox(0.8f, 0.1f, 0.8f, leatherTexture);
    glPopMatrix();

    // Back (0.8 x 0.8 x 0.1)
    glPushMatrix();
    glTranslatef(0, 0.6f, -0.35f);
    drawBox(0.8f, 1.2f, 0.1f, leatherTexture);
    glPopMatrix();

    // Legs (4 modern angled legs)
    float legPositions[4][2] = { {-0.3f, -0.3f}, {0.3f, -0.3f}, {-0.3f, 0.3f}, {0.3f, 0.3f} };
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legPositions[i][0], 0.0f, legPositions[i][1]);
        drawBox(0.05f, 0.5f, 0.05f, metalTexture);
        glPopMatrix();
    }

    glPopMatrix();
}

void FurnitureHelper::drawLuxuryConvertible(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Set luxury white material (using marble texture)
    glBindTexture(GL_TEXTURE_2D, potTexture); // marble.jpg for white luxury body
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Pure white

    float mat_luxury_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    float mat_luxury_shininess[] = { 120.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_luxury_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_luxury_shininess);

    // ===== THIN CHASSIS =====
    // Main chassis - very thin and low
    glPushMatrix();
    // Underbody - thin plate
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    drawBox(1.4f, 0.05f, 4.2f, potTexture);
    glPopMatrix();

    // Central spine for rigidity (racing inspired)
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f);
    drawBox(0.15f, 0.15f, 4.0f, chromeTexture);
    glPopMatrix();

    // Side frame rails (thin)
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.6f, 0.2f, 0.0f);
        drawBox(0.05f, 0.1f, 4.0f, chromeTexture);
        glPopMatrix();
    }
    glPopMatrix();

    // ===== BODY PANELS =====
    // Front section (hood area)
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 1.5f);
    glRotatef(+10.0f, 1.0f, 0.0f, 0.0f); // Slight downward slope
    drawBox(1.2f, 0.08f, 1.3f, potTexture);
    glPopMatrix();

    // Rear section (trunk area)
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, -1.5f);
    glRotatef(-8.0f, 1.0f, 0.0f, 0.0f); // Slight upward slope
    drawBox(1.2f, 0.08f, 1.3f, potTexture);
    glPopMatrix();

    // Side panels (very thin)
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.55f, 0.25f, 0.0f);
        drawBox(0.03f, 0.15f, 2.5f, potTexture);
        glPopMatrix();
    }

    // ===== INTERIOR (NO ROOF - CONVERTIBLE) =====
    glBindTexture(GL_TEXTURE_2D, leatherTexture);
    glColor4f(0.1f, 0.1f, 0.1f, 1.0f); // Black leather interior

    // Seats - low profile racing seats
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.35f, 0.35f, 0.0f);

        // Seat base
        drawBox(0.25f, 0.15f, 0.4f, leatherTexture);

        // Seat back
        glPushMatrix();
        glTranslatef(0.0f, 0.15f, -0.15f);
        glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
        drawBox(0.25f, 0.3f, 0.05f, leatherTexture);
        glPopMatrix();

        // Seat side bolsters
        glPushMatrix();
        glTranslatef(i * 0.1f, 0.1f, 0.0f);
        drawBox(0.05f, 0.25f, 0.35f, leatherTexture);
        glPopMatrix();

        glPopMatrix();
    }

    // Center console
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.0f);
    drawBox(0.2f, 0.15f, 1.2f, leatherTexture);

    // Gear shift
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.2f);
    drawBox(0.05f, 0.15f, 0.05f, chromeTexture);
    drawBox(0.2f, 0.03f, 0.2f, chromeTexture);
    glPopMatrix();

    // Handbrake
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glRotatef(45.0f,1.0f,0.0f,0.0f);
    drawBox(0.03f, 0.2f, 0.03f, chromeTexture);
    glPopMatrix();
    glPopMatrix();

    // Dashboard
    glPushMatrix();
    glTranslatef(0.0f, 0.45f, 0.8f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    drawBox(1.0f, 0.08f, 0.4f, leatherTexture);

    // Instrument cluster
    glPushMatrix();
    glTranslatef(0.0f, 0.04f, 0.1f);
    drawBox(0.3f, 0.12f, 0.15f, chromeTexture);
    glPopMatrix();

    // Center display
    glPushMatrix();
    glTranslatef(0.0f, 0.04f, -0.1f);
    drawBox(0.2f, 0.1f, 0.15f, chromeTexture);
    glPopMatrix();
    glPopMatrix();

    // Steering wheel
    glPushMatrix();
    glTranslatef(-0.3f, 0.45f, 0.5f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    // Steering column
    drawBox(0.04f, 0.15f, 0.04f, chromeTexture);

    // Wheel rim
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawCylinder(0.15f, 0.03f, 16, leatherTexture, 0);
    glPopMatrix();
    glPopMatrix();

    // ===== LUXURY LIGHTS =====
    glBindTexture(GL_TEXTURE_2D, chromeTexture);
    glColor4f(0.9f, 0.9f, 1.0f, 1.0f); // Silver with slight blue tint

    // FRONT LIGHTS (Detailed)
    // Main headlight housing
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.45f, 0.35f, 1.85f);

        // Outer housing (chrome)
        drawBox(0.18f, 0.12f, 0.05f, chromeTexture);

        // Inner light (glass/reflector)
        glPushMatrix();
        glColor4f(1.0f, 1.0f, 0.8f, 0.8f); // Yellowish light
        glTranslatef(0.0f, 0.0f, 0.025f);
        drawBox(0.14f, 0.08f, 0.02f, glassTexture);
        glPopMatrix();

        // LED strip below headlight
        glPushMatrix();
        glTranslatef(0.0f, -0.08f, 0.02f);
        glColor4f(0.0f, 0.8f, 1.0f, 1.0f); // Blue LED
        drawBox(0.15f, 0.02f, 0.03f, chromeTexture);
        glPopMatrix();
        glPopMatrix();
    }

    // Daytime running lights (thin LED strips)
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 1.95f);
    glRotatef(5.0f, 1.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 1.0f, 0.5f, 1.0f); // White-yellow
    drawBox(0.8f, 0.01f, 0.02f, chromeTexture);
    glPopMatrix();

    // REAR LIGHTS (Detailed)
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.45f, 0.35f, -1.85f);

        // Outer housing (chrome)
        drawBox(0.2f, 0.15f, 0.05f, chromeTexture);

        // Brake light (red)
        glPushMatrix();
        glColor4f(1.0f, 0.1f, 0.1f, 0.9f); // Red
        glTranslatef(0.0f, 0.0f, 0.025f);
        drawBox(0.16f, 0.11f, 0.02f, glassTexture);
        glPopMatrix();

        // Turn signal (amber)
        glPushMatrix();
        glTranslatef(0.0f, -0.08f, 0.025f);
        glColor4f(1.0f, 0.5f, 0.0f, 0.9f); // Amber
        drawBox(0.16f, 0.02f, 0.02f, glassTexture);
        glPopMatrix();

        // Reverse light (white)
        glPushMatrix();
        glTranslatef(0.0f, 0.08f, 0.025f);
        glColor4f(1.0f, 1.0f, 1.0f, 0.9f); // White
        drawBox(0.16f, 0.02f, 0.02f, glassTexture);
        glPopMatrix();
        glPopMatrix();
    }

    // Center high-mounted stop light
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, -1.85f);
    glColor4f(1.0f, 0.1f, 0.1f, 0.9f); // Red
    //drawBox(0.15f, 0.04f, 0.03f, glassTexture);
    glPopMatrix();

    // ===== GRILLE AND FRONT DETAILS =====
    glColor4f(0.7f, 0.7f, 0.7f, 1.0f); // Silver

    // Main grille (horizontal slats)
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 1.95f);
    for (int i = -2; i <= 2; i++) {
        glPushMatrix();
        glTranslatef(0.0f, i * 0.04f, 0.0f);
        //drawBox(0.6f, 0.008f, 0.03f, chromeTexture);
        glPopMatrix();
    }
    glPopMatrix();

    // Badge/emblem
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 2.0f);
    glColor4f(0.9f, 0.9f, 0.2f, 1.0f); // Gold
    //drawBox(0.08f, 0.08f, 0.02f, chromeTexture);
    glPopMatrix();

    // ===== SIDE DETAILS =====
    // Side vents (behind front wheels)
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.6f, 0.3f, 0.8f);
        glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
        drawBox(0.15f, 0.03f, 0.2f, chromeTexture);
        glPopMatrix();
    }

    // Door handles
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.58f, 0.35f, 0.2f);
        drawBox(0.02f, 0.04f, 0.1f, chromeTexture);
        glPopMatrix();
    }

    // Side mirrors
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.55f, 0.6f, 0.7f);

        // Mirror stem
        glPushMatrix();
        glTranslatef(0.0f, -0.15f, -0.1f);
        drawBox(0.02f, 0.15f, 0.02f, chromeTexture);
        glPopMatrix();

        // Mirror housing
        drawBox(0.1f, 0.06f, 0.03f, chromeTexture);

        // Mirror glass
        glPushMatrix();
        glTranslatef(i * 0.045f, 0.0f, 0.015f);
        glColor4f(0.8f, 0.8f, 1.0f, 0.7f);
        drawBox(0.06f, 0.04f, 0.01f, glassTexture);
        glPopMatrix();

        glPopMatrix();
    }

    // ===== REAR DETAILS =====
    // Exhaust tips (dual)
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.25f, 0.2f, -2.0f);
        glColor4f(0.3f, 0.3f, 0.3f, 1.0f); // Dark metal
        drawCylinder(0.04f, 0.15f, 12, chromeTexture, 0);
        glPopMatrix();
    }

    // Diffuser
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, -2.05f);
    glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
    //drawBox(0.8f, 0.05f, 0.1f, chromeTexture);
    glPopMatrix();

    // ===== WHEELS (LUXURY DESIGN) =====
    // Front left wheel
    drawWheel(-0.6f, 0.2f, 1.4f, 0.3f, 0.18f, -90.0f);

    // Front right wheel
    drawWheel(0.6f, 0.2f, 1.4f, 0.3f, 0.18f, 90.0f);

    // Rear left wheel
    drawWheel(-0.6f, 0.2f, -1.4f, 0.32f, 0.22f, -90.0f);

    // Rear right wheel
    drawWheel(0.6f, 0.2f, -1.4f, 0.32f, 0.22f, 90.0f);

    // Wheel arch flares (subtle)
    float archPositions[4][3] = {
        {-0.6f, 0.25f, 1.4f},
        {0.6f, 0.25f, 1.4f},
        {-0.6f, 0.25f, -1.4f},
        {0.6f, 0.25f, -1.4f}
    };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(archPositions[i][0], archPositions[i][1], archPositions[i][2]);
        glColor4f(0.1f, 0.1f, 0.1f, 0.8f);
        drawCylinder(0.33f, 0.05f, 16, chromeTexture, 90);
        glPopMatrix();
    }

    // ===== WINDSCREEN (MINIMAL) =====
    glBindTexture(GL_TEXTURE_2D, glassTexture);
    glColor4f(0.7f, 0.8f, 1.0f, 0.4f); // Light blue tint

    // Minimal windscreen
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, 0.6f);
    glRotatef(60.0f, 1.0f, 0.0f, 0.0f);
    drawBox(0.8f, 0.02f, 0.3f, glassTexture);
    glPopMatrix();

    // Windscreen frame (chrome)
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, 0.6f);
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
    drawBox(0.82f, 0.03f, 0.01f, chromeTexture);
    glPopMatrix();

    // Roll bars (safety feature for convertible)
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.25f, 0.7f, -0.3f);

        // Vertical bar
        //drawBox(0.04f, 0.3f, 0.04f, chromeTexture);

        // Horizontal crossbar
        glPushMatrix();
        glTranslatef(0.0f, 0.15f, 0.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        //drawBox(0.04f, 0.04f, 0.5f, chromeTexture);
        glPopMatrix();

        glPopMatrix();
    }

    glPopMatrix(); // End luxury car
}

void FurnitureHelper::drawFamilySUV(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Set SUV body material (red color)
    glColor4f(1.0f, 0.2f, 0.2f, 1.0f); // Bright red color
    float mat_body_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float mat_body_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_body_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_body_shininess);
    glBindTexture(GL_TEXTURE_2D, redMetalTexture);

    // ===== MAIN BODY (Simple boxy SUV design) =====

    // Lower body (wider and taller than sports car)
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 0.0f);
    drawBox(2.0f, 1.0f, 4.5f, redMetalTexture);
    glPopMatrix();

    // Upper cabin section
    glPushMatrix();
    glTranslatef(0.0f, 1.1f, -0.4f);
    drawBox(1.8f, 0.9f, 3.5f, redMetalTexture);
    glPopMatrix();

    // Hood (front section)
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 1.8f);
    drawBox(1.8f, 0.6f, 1.4f, redMetalTexture);
    glPopMatrix();

    // Trunk (rear section)
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, -1.8f);
    drawBox(1.8f, 0.8f, 1.4f, redMetalTexture);
    glPopMatrix();

    // ===== WINDOWS (Simple boxy windows) =====
    glBindTexture(GL_TEXTURE_2D, glassTexture);
    glColor4f(0.8f, 0.9f, 1.0f, 0.5f); // Light blue tinted windows

    // Front windshield
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 1.6f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    drawBox(1.5f, 0.4f, 0.8f, glassTexture);
    glPopMatrix();

    // Side windows
    // Left side window
    glPushMatrix();
    glTranslatef(-0.9f, 1.2f, -0.4f);
    drawBox(0.05f, 0.7f, 2.7f, glassTexture);
    glPopMatrix();

    // Right side window
    glPushMatrix();
    glTranslatef(0.9f, 1.2f, -0.4f);
    drawBox(0.05f, 0.7f, 2.7f, glassTexture);
    glPopMatrix();

    // Rear windshield
    glPushMatrix();
    glTranslatef(0.0f, 1.4f, -1.8f);
    drawBox(1.5f, 0.4f, 0.8f, glassTexture);
    glPopMatrix();

    // ===== BUMPERS & GRILLE =====
    glBindTexture(GL_TEXTURE_2D, metalTexture);
    glColor4f(0.3f, 0.3f, 0.3f, 1.0f); // Dark gray for bumpers

    // Front bumper
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 2.5f);
    drawBox(1.9f, 0.3f, 0.2f, metalTexture);
    glPopMatrix();

    // Rear bumper
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, -2.5f);
    drawBox(1.9f, 0.3f, 0.2f, metalTexture);
    glPopMatrix();

    // Front grille (simple horizontal bars)
    glPushMatrix();
    glTranslatef(0.0f, 0.9f, 2.5f);
    for (int i = -1; i <= 1; i++) {
        glPushMatrix();
        glTranslatef(0.0f, i * 0.15f, 0.0f);
        drawBox(1.2f, 0.03f, 0.05f, metalTexture);
        glPopMatrix();
    }
    glPopMatrix();

    // ===== LIGHTS (Simple rectangular lights) =====
    glBindTexture(GL_TEXTURE_2D, chromeTexture);

    // Headlights
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.7f, 0.9f, 2.5f);
        glColor4f(1.0f, 1.0f, 0.8f, 1.0f); // Yellowish light color
        drawBox(0.25f, 0.15f, 0.05f, chromeTexture);
        glPopMatrix();
    }

    // Taillights
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.7f, 0.9f, -2.5f);
        glColor4f(1.0f, 0.1f, 0.1f, 1.0f); // Red light color
        drawBox(0.25f, 0.2f, 0.05f, chromeTexture);
        glPopMatrix();
    }

    // ===== WHEELS (SUV wheels - slightly larger) =====
    // Front left wheel
    drawWheel(-0.85f, 0.45f, 1.5f, 0.4f, 0.25f, -90.0f);

    // Front right wheel
    drawWheel(0.85f, 0.45f, 1.5f, 0.4f, 0.25f, 90.0f);

    // Rear left wheel
    drawWheel(-0.85f, 0.45f, -1.5f, 0.4f, 0.25f, -90.0f);

    // Rear right wheel
    drawWheel(0.85f, 0.45f, -1.5f, 0.4f, 0.25f, 90.0f);

    // ===== ROOF RAILS (SUV feature) =====
    glBindTexture(GL_TEXTURE_2D, metalTexture);
    glColor4f(0.4f, 0.4f, 0.4f, 1.0f);

    // Left roof rail
    glPushMatrix();
    glTranslatef(-0.8f, 2.1f, 0.0f);
    drawBox(0.1f, 0.05f, 3.0f, metalTexture);
    glPopMatrix();

    // Right roof rail
    glPushMatrix();
    glTranslatef(0.8f, 2.1f, 0.0f);
    drawBox(0.1f, 0.05f, 3.0f, metalTexture);
    glPopMatrix();

    // ===== SIDE MIRRORS =====
    glBindTexture(GL_TEXTURE_2D, chromeTexture);
    glColor4f(0.7f, 0.7f, 0.7f, 1.0f);

    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 1.0f, 1.5f, 1.2f);
        drawBox(0.22f, 0.5f, 0.05f, chromeTexture);
        glPopMatrix();
    }

    // ===== DOOR HANDLES =====
    glColor4f(0.5f, 0.5f, 0.5f, 1.0f);

    // Front door handles
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.9f, 1.2f, 0.7f);
        drawBox(0.02f, 0.05f, 0.15f, metalTexture);
        glPopMatrix();
    }

    // Rear door handles
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.9f, 1.2f, -0.7f);
        drawBox(0.02f, 0.05f, 0.15f, metalTexture);
        glPopMatrix();
    }

    glPopMatrix(); // End SUV
}

void FurnitureHelper::drawMiniRaceCar(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Set car body material (black using cobblestone texture)
    glBindTexture(GL_TEXTURE_2D, leatherTexture); // Using cobblestone.jpg
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // White tint for texture

    float mat_body_specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float mat_body_shininess[] = { 30.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_body_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_body_shininess);

    // ===== SIMPLE BODY =====
    // Main chassis (very simple rectangular shape)
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.0f);
    drawBox(1.0f, 0.3f, 2.0f, leatherTexture); // Main body
    glPopMatrix();

    // Hood (front section - slightly lower)
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 1.0f);
    drawBox(0.8f, 0.2f, 0.6f, leatherTexture);
    glPopMatrix();

    // Rear section
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, -1.0f);
    drawBox(0.8f, 0.2f, 0.6f, leatherTexture);
    glPopMatrix();

    // ===== SIMPLE ROLL CAGE =====
    glPushMatrix();
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f); // Gray metal
    glBindTexture(GL_TEXTURE_2D, metalTexture);

    // Front roll bar
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, 0.5f);
    drawBox(0.7f, 0.05f, 0.05f, metalTexture);
    glPopMatrix();

    // Rear roll bar
    glPushMatrix();
    glTranslatef(0.0f, 0.6f, -0.5f);
    drawBox(0.7f, 0.05f, 0.05f, metalTexture);
    glPopMatrix();

    // Side roll bars
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.35f, 0.4f, 0.0f);
        drawBox(0.05f, 0.3f, 1.0f, metalTexture);
        glPopMatrix();
    }
    glPopMatrix();

    // ===== SIMPLE FRONT DETAILS =====
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, chromeTexture);
    glColor4f(0.9f, 0.9f, 0.9f, 1.0f);

    // Simple front bumper
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 1.15f);
    drawBox(0.7f, 0.1f, 0.05f, chromeTexture);
    glPopMatrix();

    // Simple headlights
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.25f, 0.3f, 1.15f);
        drawBox(0.1f, 0.08f, 0.05f, chromeTexture);
        glPopMatrix();
    }
    glPopMatrix();

    // ===== SIMPLE REAR DETAILS =====
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, redMetalTexture);
    glColor4f(1.0f, 0.3f, 0.3f, 1.0f); // Red color

    // Simple rear bumper
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, -1.15f);
    drawBox(0.7f, 0.1f, 0.05f, redMetalTexture);
    glPopMatrix();

    // Simple taillights
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.25f, 0.3f, -1.15f);
        drawBox(0.1f, 0.08f, 0.05f, redMetalTexture);
        glPopMatrix();
    }
    glPopMatrix();

    // ===== SIMPLE WING/SPOILER =====
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, redMetalTexture);
    glColor4f(1.0f, 0.3f, 0.3f, 1.0f); // Red

    // Main wing
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, -0.8f);
    drawBox(0.8f, 0.05f, 0.3f, redMetalTexture);
    glPopMatrix();

    // Wing supports
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(i * 0.3f, 0.35f, -0.8f);
        drawBox(0.05f, 0.2f, 0.05f, redMetalTexture);
        glPopMatrix();
    }
    glPopMatrix();

    // ===== SIDE EXHAUST =====
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, chromeTexture);
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);

    glPushMatrix();
    glTranslatef(0.4f, 0.25f, -0.5f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawCylinder(0.05f, 0.3f, 12, chromeTexture, 0);
    glPopMatrix();
    glPopMatrix();

    // ===== WHEELS =====
    // Front left wheel
    drawWheel(-0.45f, 0.15f, 0.8f, 0.2f, 0.15f, -90.0f);

    // Front right wheel
    drawWheel(0.45f, 0.15f, 0.8f, 0.2f, 0.15f, 90.0f);

    // Rear left wheel
    drawWheel(-0.45f, 0.15f, -0.8f, 0.22f, 0.18f, -90.0f);

    // Rear right wheel
    drawWheel(0.45f, 0.15f, -0.8f, 0.22f, 0.18f, 90.0f);

    glPopMatrix(); // End mini race car
}

void FurnitureHelper::drawInfoKiosk(float x, float y, float z, float rotation, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    glScalef(scale, scale, scale);

    // Main kiosk body (1.2 x 1.8 x 0.8)
    glPushMatrix();
    glTranslatef(0, 0.2f, 0);
    drawBox(1.2f, 2.2f, 0.8f, metalTexture);
    glPopMatrix();

    // Top panel (1.5 x 0.1 x 1.0)
    glPushMatrix();
    glTranslatef(0, 1.3f, 0);
    drawBox(1.5f, 0.1f, 1.0f, metalTexture);
    glPopMatrix();

    // Base (1.5 x 0.2 x 1.0)
    glPushMatrix();
    drawBox(1.5f, 0.2f, 1.0f, metalTexture);
    glPopMatrix();

    // Screen area (0.8 x 0.6 x 0.05)
    glPushMatrix();
    glTranslatef(0, 1.5f, 0.41f);
    drawBox(0.8f, 0.6f, 0.05f, potTexture);
    glPopMatrix();

    glPopMatrix();
}