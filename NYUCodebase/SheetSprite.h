#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SheetSprite {
public:
	~SheetSprite();
	SheetSprite(float u, float v, float width, float height);
	SheetSprite();

	float u;
	float v;
	float width;
	float height;
};

SheetSprite::~SheetSprite(){}
SheetSprite::SheetSprite(){}
SheetSprite::SheetSprite(float u, float v, float width, float height) : u(u), v(v), width(width), height(height) {}
