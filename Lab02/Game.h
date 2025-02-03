#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>
#include "SpriteComponent.h"
#include "Actor.h"
#include "Asteroid.h"

class Game
{
public:
	Game() noexcept;
	bool Initialize();
	void RunLoop();
	void Shutdown();
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	SDL_Texture* GetTexture(const std::string& fileName); // we don't want the filename to change.
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	void AddAsteroid(class Asteroid* asteroid);
	void RemoveAsteroid(class Asteroid* asteroid);
	const std::vector<Asteroid*>& GetAsteroid() const
	{
		return mAsteroid;
	} // use the template in actor.h
	// for revising the magic number in Asteroid.h
	int GetWindowWidth() const { return WINDOW_WIDTH; }
	int GetWindowHeight() const { return WINDOW_HEIGHT; }

private:
	SDL_Window* mWindow;	 // window: SDL_CreateWindow
	SDL_Renderer* mRenderer; // renderer: SDL_CreateRenderer
	bool mIsLoop;			 // can it go into loop?
	//
	//
	// __________________________________________________________________________
	//
	//
	// window setting
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;
	//
	//
	// _________________________________________________________________________
	//
	// Delta time and Movement (for Ball and Paddle)
	// dt time
	Uint32 mPrevTime; // time from prev frm

	//
	//
	// _________________________________________________________________________
	//
	//
	bool InitSDL(Uint32 flags);
	SDL_Window* CreateSDLWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
	SDL_Renderer* CreateSDLRenderer(SDL_Window* window, int index, Uint32 flags);
	//
	//
	// _______________________________________________________________________
	//
	//
	// subpart of run loop
	void ProcessInput();
	void GenerateOutput();
	void UpdateGame();
	//
	//
	// ___________________________________________________________________________
	//
	//
	// hook up actors to the game, forward declaration for all class pointers in header files!!!
	std::vector<class Actor*> mActors;
	//
	//
	//_____________________________________________________________________________
	//
	//
	// Caching Loaded Textures
	std::unordered_map<std::string, SDL_Texture*> mTexturePrototype;
	//
	// _____________________________________________________________________________
	//
	//
	// load data and unload
	void LoadData();
	void UnloadData();
	//
	//
	//_______________________________________________________________________________
	//
	//
	// SpriteComponent
	std::vector<class SpriteComponent*> mSprite;
	//
	//
	//_____________________________________________________________________________
	//
	// Lasers Colliding with Asteroids
	std::vector<class Asteroid*> mAsteroid; // track all the asteroids

	//
	//
	//_____________________________________________________________________________
	//
	// Starship and star parameter setting.
	Vector2 mStarShipPos = Vector2(512.0f, 384.0f);
	float mStarShipSpeed = 250.0f;
	Vector2 mStarNightPos = Vector2(512.0f, 384.0f);
};

// make sure to construc the function in game.cpp in game.h order
