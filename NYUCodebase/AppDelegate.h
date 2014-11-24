#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <string>
//#include "Entity.h"

using namespace std;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID); // how many names to generate, and address of arrays where names go
	glBindTexture(GL_TEXTURE_2D, textureID); // says, gltexture2d is bound to textureid now
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);//sets texture data of target
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//target, property, property definition, sets the gltextureminfilter property of gltexture2d to gllinear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SDL_FreeSurface(surface);
	return textureID;
}
GLuint LoadTexture24(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID); // how many names to generate, and address of arrays where names go
	glBindTexture(GL_TEXTURE_2D, textureID); // says, gltexture2d is bound to textureid now
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);//sets texture data of target
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//target, property, property definition, sets the gltextureminfilter property of gltexture2d to gllinear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SDL_FreeSurface(surface);
	return textureID;
}
void DrawText(int fontTexture, string text, float size, float spacing, float r, float g, float b, float a,float x, float y) {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, fontTexture); // so gltexture_2d knows what textureid to draw
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0);
	float texture_size = 1.0 / 16.0f;

	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });

		vertexData.insert(vertexData.end(), { ((size + spacing)*i) + (-0.5f*size), .5f*size,
			((size + spacing)*i) + (-.5f*size), -.5f*size,
			((size + spacing)*i) + (.5f*size), -.5f*size,
			((size + spacing)*i) + (.5f*size), .5f*size
		});

		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_BLEND);
}

class AppDelegate {
	public:
		SDL_Event event;
		int score = 0;
		bool win = false;
		float sincelastshottime = 0;

		enum GameState {STATE_MAIN_MENU,STATE_GAME_LEVEL,STATE_GAME_OVER};
		GameState state;
		AppDelegate();
		~AppDelegate();

		//Textures
		GLuint invadersid,fontid;

		//SheetSprites, already initialized
		SheetSprite mainmenusprite = SheetSprite(443.0f / 585.0f, 503.0f / 718.0f, 127.0f / 585.0f, 88.0f / 718.0f);
		SheetSprite beamsprite = SheetSprite(404.0f / 585.0f, 393.0f / 718.0f, 24.0f / 585.0f, 51.0f / 718.0f);
		SheetSprite invaderlasersprite = SheetSprite(482.0f / 585.0f, 391.0f / 718.0f, 37.0f / 585.0f, 57.0f / 718.0f);
		SheetSprite invadersprite = SheetSprite(13.0f / 585.0f, 12.0f / 718.0f, 119.0f / 585.0f, 85.0f / 718.0f);
		SheetSprite shipsprite = SheetSprite(147.0f / 585.0f, 635.0f / 718.0f, 79.0f / 585.0f, 59.0f / 718.0f);

		//Entities
		Entity mainmenu;
		vector<Entity*> invaders;
		Entity ship;
		vector<Entity*> invaderlasers;
		vector<Entity*> beams;

		void Init();
		bool UpdateAndRender();
		void Render();
		void RenderMainMenu();
		void RenderGameLevel();
		void RenderGameOver();
		void Update(float elapsed);
		void UpdateMainMenu(float elapsed);
		void UpdateGameLevel(float elapsed);
		void UpdateGameOver(float elapsed);
		bool Collision(Entity A, Entity B);
	private:
		bool done;
		float lastFrameTicks;
		SDL_Window* displayWindow;
};

AppDelegate::AppDelegate(){
	Init();
	done = false;
	lastFrameTicks = 0.0;
}

void AppDelegate::Init(){
	SDL_Init(SDL_INIT_VIDEO); //SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK); // for initing and taking over joystick too. if not also quitte
	displayWindow = SDL_CreateWindow("Simon Project 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glClearColor(0.2, 0.0, 0.0, 1.0);//background color 

	//Setup
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//LoadTextures
	invadersid = LoadTexture("pngs/invaders.png");
	fontid = LoadTexture("pngs/font.png");
	
	//Create/Init Entities
	mainmenu = Entity(mainmenusprite, invadersid, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 2.66f, 2.f);
	ship = Entity(shipsprite, invadersid, 0.f, -1+.025f, 1.f, .3, 0.7f, .0f, .2f, .1f);
	//make invaders
	for (int i = 0; i < 10; i++)
	{
		float x = -1.33 + (2.66 / 6)*(i % 5 + 1);
		float y = (1.0f / 3.0)*((i / 5) + 1);
		Entity* newinv = new Entity(invadersprite, invadersid, x, y, 1.f, 0.f, 0.4f, 0.f, .2f, .2f);
		invaders.push_back(newinv);
	}

	//set State
	state = STATE_MAIN_MENU;

}

AppDelegate::~AppDelegate() {
	SDL_Quit();
	//SDL_JoystickClose(playerOneController);// close joystick, necessary or joystick is locked forever
}

bool AppDelegate::Collision(Entity A, Entity B)
{
	float A_top = A.y + (A.height / 2.0f);
	float A_bot = A.y - (A.height / 2.0f);
	float A_right = A.x + (A.width / 2.0f);
	float A_left = A.x - (A.width / 2.0f);

	float B_top = B.y + (B.height / 2.0f);
	float B_bot = B.y - (B.height / 2.0f);
	float B_right = B.x + (B.width / 2.0f);
	float B_left = B.x - (B.width / 2.0f);

	// Based on the rules for collision detection we have:
	if (A_bot > B_top) {
		return false;
	}
	else if (A_top < B_bot) {
		return false;
	}
	else if (A_left > B_right){
		return false;
	}
	else if (A_right < B_left) {
		return false;
	}

	// A true is returned if there has been a collision
	return true;
}

void AppDelegate::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state)
	{
		case STATE_MAIN_MENU:
			RenderMainMenu();
			break;
		case STATE_GAME_LEVEL:
			RenderGameLevel();
			break;
		case STATE_GAME_OVER:
			RenderGameOver();
			break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

void AppDelegate::Update(float elapsed) {
	sincelastshottime += elapsed;
	switch (state) {
		case STATE_MAIN_MENU:
			UpdateMainMenu(elapsed);
			break;
		case STATE_GAME_LEVEL:
			UpdateGameLevel(elapsed);
			break;
		case STATE_GAME_OVER:
			UpdateGameOver(elapsed);
			break;
	}
}

bool AppDelegate::UpdateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	Update(elapsed);
	Render();
	return done;
}

void AppDelegate::RenderMainMenu(){
	mainmenu.Draw();
	DrawText(fontid, "Press Space to Start", .05f, 0.0f, 1.f, 1.f, 1.f, 1.f,-.5f,-.8f);
}
void AppDelegate::RenderGameLevel(){
	//DrawText(fontid, "Game Started", .05, 0.0, 1, 1, 1, 1, -.5, -.8);
	//DrawText(fontid, std::to_string(invaders.size()), .05f, 0.0f, 1.f, 1.f, 1.f, 1.f, -.5f, -.8f);
	ship.Draw();
	for (unsigned int i = 0; i < invaders.size(); i++)
	{
		invaders[i]->Draw();
	}
	for (unsigned int i = 0; i < beams.size(); i++)
	{
		beams[i]->Draw();
	}
	for (unsigned int i = 0; i < invaderlasers.size(); i++)
	{
		invaderlasers[i]->Draw();
	}
}
void AppDelegate::RenderGameOver(){
	DrawText(fontid, std::to_string(score), .05f, 0.0f, 1.f, 1.f, 1.f, 1.f, -.8f, -.8f);
	if (win)
	{
		DrawText(fontid, "Game Over YOU WIN", .05f, 0.0f, 1.f, 1.f, 1.f, 1.f, -.8f, .8f);
	}
	else
	{
		DrawText(fontid, "Game Over YOU SUCK", .05f, 0.0f, 1.f, 1.f, 1.f, 1.f, -.8f, .8f);
	}
}
void AppDelegate::UpdateMainMenu(float elapsed)
{
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE&& state == STATE_MAIN_MENU) {
				state = STATE_GAME_LEVEL;
			}
		}
	}
}
void AppDelegate::UpdateGameLevel(float elapsed)
{
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN){
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (sincelastshottime > .3)
				{
					Entity * newbm = new Entity(beamsprite, invadersid, ship.x, -1 + .25, 1, 0, 0, .8, .05, .1);
					beams.push_back(newbm);
					sincelastshottime = 0.0f;
				}
			}
		}
	}
	//update ship
		//get input, key state for left right movement.
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	DrawText(fontid, std::to_string(ship.velocity_x), .05, 0.0, 1, 1, 1, 1, .5, .5);
	if (keys[SDL_SCANCODE_RIGHT]) {
		ship.x += ship.velocity_x*elapsed;
		if (ship.x > 1.33f){ ship.x = 1.33f; }
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		ship.x -= ship.velocity_x*elapsed;
		if (ship.x < -1.33f){ ship.x = -1.33f; }
	}

	//update invaders
		//if invaders.size()==0; gg you win
		//move them.
			//if x > 1.33f, move y down by (num), velocity*=-1;
			//else if x<-1.33f, move y down by (num),velocity*=-1;
		// they have a probability of shooting a laser. small probability at every moment
	if (invaders.size() == 0){ state = STATE_GAME_OVER; win = true; }
	for (unsigned int i = 0; i < invaders.size(); i++)
	{
		if (invaders[i]->y < -1 + (invaders[i]->height / 2.0f))
		{
			state = STATE_GAME_OVER;
		}
		if (invaders[i]->x>1.33f - (invaders[i]->width/2.0f))
		{
			float delt = invaders[i]->x - (1.33f - (invaders[i]->width / 2.0f));
			for (unsigned int j = 0; j < invaders.size(); j++)
			{
				invaders[j]->x = invaders[j]->x - delt;
				invaders[j]->y -= .08;
				invaders[j]->velocity_x *= -1.0f;
			}
			break;
		}
		if (invaders[i]->x<-1.33f + (invaders[i]->width / 2.0f))
		{
			float delt = (-1.33f + (invaders[i]->width / 2.0f))-invaders[i]->x;// -1.33 - invaders[i]->x;
			for (unsigned int j = 0; j < invaders.size(); j++)
			{
				invaders[j]->x = invaders[j]->x + delt;
				invaders[j]->y -= .08;
				invaders[j]->velocity_x *= -1.0f;
			}
			break;
		}
		// they have a probability of shooting a laser. small probability at every moment// 5/1000
		float prob = (rand() % 40000);
		if (prob < 5)
		{
			Entity * newlaser = new Entity(invaderlasersprite, invadersid, invaders[i]->x, invaders[i]->y - (invaders[i]->height/2.0f), 1, 0, 0, -.4, .05, .1);
			invaderlasers.push_back(newlaser);
		}
		invaders[i]->Update(elapsed);
	}

	//update beams
	for (unsigned int i = 0; i < beams.size(); i++)
	{
		beams[i]->Update(elapsed);
		//if beam is above 1 in y value, erase it.
		if (beams[i]->y>1.0 - (beams[i]->height / 2.0f))
		{
			beams.erase(beams.begin() + i);
			i--;
		}
		else
		{
			// if beam is touching any invader in loop. destroy beam, destroy invader, score++;
			for (unsigned int j = 0; j<invaders.size(); j++)
			{
				if (Collision(*invaders[j], *beams[i]))
				{
					beams.erase(beams.begin() + i);
					invaders.erase(invaders.begin() + j);
					score++; i--;
					break;
				}
			}
		}
	}

	//update invaderlasers
		//if laser touches ship. game over you dead
	for (unsigned int i = 0; i < invaderlasers.size(); i++)
	{
		// if laser is touching ship, game state over, win=false
		invaderlasers[i]->Update(elapsed);
		if (invaderlasers[i]->y<-1.0 + (invaderlasers[i]->height / 2.0f))
		{
			invaderlasers.erase(invaderlasers.begin() + i);
			i--;
		}
		else if (Collision(*invaderlasers[i], ship))
		{
			state = STATE_GAME_OVER;
			break;
		}
	}
}
void AppDelegate::UpdateGameOver(float elapsed)
{
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
}