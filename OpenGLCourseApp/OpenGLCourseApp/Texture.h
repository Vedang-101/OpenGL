#pragma once

#include <GL/glew.h>
#include "CommonValues.h"

class Texture {
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char *fileLocation;
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture(bool alpha = true);
	void UseTexture();
	void ClearTexture();

	~Texture();
};

