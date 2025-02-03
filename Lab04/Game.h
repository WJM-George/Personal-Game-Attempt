#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "Random.h"
#include "Actor.h"

class Game
{
public:
	Game() noexcept;
	bool Initialize();
	void RunLoop();
	void Shutdown();
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	// public for getting
	void AddBlock(class Block* block);
	void RemoveBlock(class Block* block);
	const std::vector<class Block*>& GetBlock() const { return mBlock; }
	// for destination checking
	Actor* GetGoal() const { return mGoal; }
	class CollisionComponent* GetGoalColliComp() const { return mGoalColliComp; }
	// avoid reuse of same parameter:
	int GetWindowWidth() const { return WINDOW_WIDTH; }
	int GetWindowHeight() const { return WINDOW_HEIGHT; }
	class Player* GetPlayer() const { return mPlayer; }
	// getter function for camera:
	Vector2& GetCameraPos() { return mCameraPosition; }
	//Goomba setting:
	void AddGoomba(class Goomba* goomba);
	void RemoveGoomba(class Goomba* goomba);
	const std::vector<class Goomba*>& GetGoomba() const { return mGoomba; }
	// for the sound and texture:
	SDL_Texture* GetTexture(const std::string& fileName);
	Mix_Chunk* GetSound(const std::string& filename);
	// for getting elsewhere.
	int mMusicChannel;

private:
	SDL_Window* mWindow;	 // window: SDL_CreateWindow
	SDL_Renderer* mRenderer; // renderer: SDL_CreateRenderer
	bool mIsLoop;
	//
	//
	// __________________________________________________________________________
	//
	//
	// window setting
	const int WINDOW_WIDTH = 600;  // should be 600
	const int WINDOW_HEIGHT = 448; // should be 448
	const int BACKGROUND_WIDTH = 3392;
	const int BACKGROUND_HEIGHT = 224;
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
	// _________________________________________________________________________
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
	// Caching Loaded Textures and music:
	std::unordered_map<std::string, SDL_Texture*> mTexturePrototype;
	std::unordered_map<std::string, Mix_Chunk*> mSoundPrototype;
	//
	// _____________________________________________________________________________
	//
	//
	// load data and unload
	void LoadData();
	void UnloadData();
	//
	//
	//______________________________________________________________________________
	//
	//
	// SpriteComponent
	std::vector<class SpriteComponent*> mSprite;
	//
	//
	//______________________________________________________________________________
	//
	std::vector<class Block*> mBlock;
	std::vector<class Goomba*> mGoomba;
	Actor* mGoal; // for destination // if member, easy to get from
	class CollisionComponent* mGoalColliComp;
	class Player* mPlayer;
	//
	//
	//---------------------------------------------------------------------------------
	//
	//
	const Vector2 GOAL_POS = Vector2(224.0f, 80.0f);
	const float GOAL_COLLICOMP_SIZE = 32.0f;
	//
	//
	//---------------------------------------------------------------------------------
	//
	//for camera scrolling
	Vector2 mCameraPosition;
	//
	//
	// for game winning sound:
};

// make sure to construc the function in game.cpp in game.h order
