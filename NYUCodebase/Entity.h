#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <string>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include "SheetSprite.h"
#include "SDL_main.h"

class Entity {
public:
	~Entity();
	Entity();
	Entity(SheetSprite sprite, int textureID, float x, float y, float scale, float rotation, float velocity_x, float velocity_y, float width, float height);

	void Draw();
	void Update(float elapsed);
	SheetSprite sprite;
	float x = 0;
	float y = 0;
	float rotation = 0;
	float scale = 1.0;
	int textureID;

	float width; // width on game screen
	float height;
	float velocity_x = 0;
	float velocity_y = 0;


};

Entity::~Entity(){}
Entity::Entity(){}
Entity::Entity(SheetSprite sprite, int textureID, float x, float y, float scale, float rotation, float velocity_x, float velocity_y, float width, float height)
{
	Entity::textureID = textureID;
	Entity::x = x;
	Entity::y = y;
	Entity::scale = scale;
	Entity::rotation = rotation;
	Entity::velocity_x = velocity_x;
	Entity::velocity_y = velocity_y;
	Entity::width = width;
	Entity::height = height;
	Entity::sprite = sprite;

}
void Entity::Update(float elapsed)
{
	x += elapsed*velocity_x;
	y += elapsed*velocity_y;
}
void Entity::Draw() {
	//init
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0, 0, 1);
	glScalef(scale, scale, scale);

	GLfloat vertices[] = { -width / 2.f, height / 2.f,
		-width / 2.f, -height / 2.f,
		width / 2.f, -height / 2.f,
		width / 2.f, height / 2.f };

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);


	GLfloat quadUVs[] = { sprite.u, sprite.v, 
						  sprite.u, sprite.v + sprite.height, 
						  sprite.u + sprite.width, sprite.v + sprite.height, 
						  sprite.u + sprite.width, sprite.v 
	};

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_BLEND);
}