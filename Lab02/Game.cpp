//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include <algorithm>
#include "SpriteComponent.h"
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "MoveComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include <string>
#include "Random.h"
#include <iostream>
#include <vector>

Game::Game() noexcept
: mWindow(nullptr)
, mRenderer(nullptr)
, mIsLoop(true)
, mPrevTime(0)
{
}

//real constructor?
bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) //2 subsystem.
	{
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game: Lab 2 Asteroid", SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (!mWindow) //check window
	{
		SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
		return false;
	}

	// create SDL render // with third para
	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// check again
	if (!mRenderer)
	{
		SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
		return false;
	}

	//initialize SDL Image with IMG_Init
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("IMG_Init Error: %s", IMG_GetError()); // similar to above ones
		return false;
	}

	LoadData(); // after all systems

	return true; // finally good to go
}

//real destructor??
void Game::Shutdown()
{
	UnloadData();
	IMG_Quit(); //
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

// break the game process by parts:
void Game::RunLoop()
{
	// Game loop belowww (infinite)
	while (mIsLoop)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{ // poll until all events are handled!
	  // decide what to do with this event.
		if (event.type == SDL_QUIT)
		{
			mIsLoop = false; // 'X' button
		}
	}

	const Uint8* state =
		SDL_GetKeyboardState(nullptr); // can be the representative of all the keyboard action

	if (state[SDL_SCANCODE_ESCAPE]) // ESC for exit
	{
		mIsLoop = false;
	}

	std::vector<Actor*> copyActorinput = mActors;
	for (Actor* actor : copyActorinput)
	{
		actor->ProcessInput(
			state); // state here same -> keyState in Actor.cpp ProcessInput function
	}
}

void Game::GenerateOutput()
{
	// scene of the game
	// render color to blue
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	// clear buffer
	SDL_RenderClear(mRenderer);

	// asteroids
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // white one

	//loop over the sprite component vector

	//include the parent class header file at first. // auto does the automatic iteration
	for (auto sprite : mSprite) // if we want to use components in child class
	{
		if (sprite->IsVisible())
		{
			sprite->Draw(mRenderer); // if visible, Draw
		}
	}

	// present all
	SDL_RenderPresent(mRenderer);
}

void Game::UpdateGame()
{
	// game overall logic?
	//1. Frame limit
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mPrevTime + 16))
		;

	//2. Calculate delta time(as a float in seconds)
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - mPrevTime) / 1000.0f;

	//3. Cap to maximum delta time
	if (deltaTime > 0.033f)
	{
		deltaTime = 0.033f;
	}
	//update time
	mPrevTime = currentTime;
	//check just in case
	//SDL_Log("Delta Time: %f", deltaTime);

	//Updating the actors
	std::vector<Actor*> copyActor = mActors; //for storage
	for (Actor* actor : copyActor)
	{
		actor->Update(deltaTime);
	}

	std::vector<Actor*> destroyActor;
	for (Actor* actor : mActors) // need to destroy the actual ones
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActor.push_back(actor); // destroy the actor with state(destroy)
		}
	}

	for (Actor* actor : destroyActor)
	{
		delete actor;
	}
}

// add and remove actor below

void Game::AddActor(Actor* actor)
{
	mActors.push_back(actor); // add actor to vector: std::vector<class Actor*> mActors;
}

void Game::RemoveActor(Actor* actor) // to remove recently added vector.
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor); //use iter to find such actor
	if (iter != mActors.end())
	// if !=, then actor found.
	//because actor is not inside of mActors, it's newly added
	{
		mActors.erase(iter);
	}
}

// load and unload data below
void Game::LoadData()
{
	Ship* starship = new Ship(this);
	starship->SetPosition(mStarShipPos);

	// enemy: asteroids
	for (size_t i = 0; i < 10; i++)
	{
		new Asteroid(this);
	}

	// fourth actor: starray night
	Actor* background = new Actor(this);
	background->SetPosition(mStarNightPos);
	SpriteComponent* sc4 = new SpriteComponent(background, 50); //(?,drawOrder), two attribute
	sc4->SetTexture(GetTexture("Assets/Stars.png"));
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		Actor* actor = mActors.back(); //
		mActors.pop_back();
		delete actor;
	}

	for (auto& correspond : mTexturePrototype) // for all things in texture prototype
	{
		SDL_DestroyTexture(correspond.second); // in every map, destroy the texture
	}

	// similar to the things we did in actor.cpp. (~Actor)

	mTexturePrototype.clear(); //clear the map
}

// Caching Loaded Textures
SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	auto iter = mTexturePrototype.find(fileName); // find things in a hash map

	if (iter != mTexturePrototype.end()) // if not at the end, then we find it. it is in map
	{
		return iter->second; // second, because setting of the hashmap is
							 // fileName (first:key) -->> Texture (second:value)
	}

	SDL_Surface* mSurface = IMG_Load(fileName.c_str()); // std::string -> C-style string pointer
	if (!mSurface)
	{
		SDL_Log("Can't load image, %s, error: %s", fileName.c_str(), IMG_GetError());
		return nullptr;
	} // for debug

	// Surface -> Texture
	SDL_Texture* mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
	SDL_FreeSurface(mSurface);

	// Once you have the SDL_Texture* pointer -->>
	if (!mTexture)
	{
		SDL_Log("Can't create texture from surface, %s, error: %s", fileName.c_str(),
				SDL_GetError());
		return nullptr; // Return nullptr, like above. for debug purpose
	}

	mTexturePrototype[fileName] =
		mTexture;	 // when texture correct, add texture to Texture prototype
					 // entry to the map for that file
	return mTexture; // return the texture we want.
}

// SpriteComponent
void Game::AddSprite(SpriteComponent* sprite)
{
	mSprite.push_back(sprite); // add sprite to vector;
	std::sort(mSprite.begin(), mSprite.end(), [](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder(); // this is a lambda expression in instructions
	});
}

void Game::RemoveSprite(SpriteComponent* sprite) //similar to remove actor
{
	auto iter = std::find(mSprite.begin(), mSprite.end(), sprite);
	if (iter != mSprite.end())
	{
		mSprite.erase(iter);
	}
}

//similar to AddActor/RemoveActor
void Game::AddAsteroid(Asteroid* asteroid)
{
	mAsteroid.push_back(asteroid);
}

void Game::RemoveAsteroid(Asteroid* asteroid)
{
	auto iter = std::find(mAsteroid.begin(), mAsteroid.end(), asteroid);
	if (iter != mAsteroid.end())
	{
		mAsteroid.erase(iter);
	}
}
