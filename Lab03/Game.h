#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
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
	SDL_Texture* GetTexture(const std::string& fileName); // we don't want the filename to change.
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	// public for getting
	void AddVehicle(class Vehicle* vehicle);
	void RemoveVehicle(class Vehicle* vehicle);
	const std::vector<class Vehicle*>& GetVehicle() const
	{
		return mVehicle;
	} // use the template in actor.h // for getting all the vehicles
	class Frog* GetFrog() { return mFrog; }
	// Log similar to Vehicle:
	void AddLog(class Log* log);
	void RemoveLog(class Log* log);
	const std::vector<class Log*>& GetLog() const { return mLog; }
	// for destination checking
	Actor* GetGoal() const { return mGoal; }
	class CollisionComponent* GetGoalColliComp() const { return mGoalColliComp; }
	// avoid reuse of same parameter:
	int GetWindowWidth() const { return WINDOW_WIDTH; }
	int GetWindowHeight() const { return WINDOW_HEIGHT; }

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
	const int WINDOW_WIDTH = 448;
	const int WINDOW_HEIGHT = 512;
	//
	//
	// __________________________________________________________________________
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
	//______________________________________________________________________________
	//
	//
	// SpriteComponent
	std::vector<class SpriteComponent*> mSprite;
	//
	//
	//______________________________________________________________________________
	//
	//For finishing the frog and the vehicle relation// and log
	std::vector<class Vehicle*> mVehicle;
	class Frog* mFrog;
	std::vector<class Log*> mLog;
	Actor* mGoal; // for destination // if member, easy to get from
	class CollisionComponent* mGoalColliComp;
	//
	//
	//---------------------------------------------------------------------------------
	//
	const float GOAL_COLLICOMP_SIZE = 32.0f;
};

// make sure to construc the function in game.cpp in game.h order
