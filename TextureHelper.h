#pragma once

#include <windows.h>
#include <gl.h>
#include <glu.h>
#include "stb_image.h"
class TextureHelper
{
public:
	TextureHelper();
	~TextureHelper();

	GLuint static LoadTexture(const char* filename);
};

