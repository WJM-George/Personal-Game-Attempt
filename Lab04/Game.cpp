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
#include <SDL2/SDL_image.h>
#include "CollisionComponent.h"
#include <fstream>
#include "Block.h"
#include "Player.h"
#include "PlayerMove.h"
#include "Goomba.h"
#include "Spawner.h"

//
Game::Game() noexcept
: mMusicChannel(0)
, mWindow(nullptr)
, mRenderer(nullptr)
, mIsLoop(true)
, mPrevTime(0)
, mGoal(nullptr)
, mGoalColliComp(nullptr)
, mPlayer(nullptr)
{
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) //2 subsystem.
	{
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game: Lab 4 Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   WINDOW_WIDTH, WINDOW_HEIGHT, 0);

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
		SDL_Log("IMG_Init Error: %s", IMG_GetError());
		return false;
	}

	Random::Init();
	// for sound
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData(); // after all systems

	return true; // finally good to go
}

void Game::Shutdown()
{
	Mix_CloseAudio();
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

// break the game process by parts:
void Game::RunLoop()
{
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
	// render color to black
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	// clear buffer
	SDL_RenderClear(mRenderer);

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
	//Mario background:
	Actor* background = new Actor(this);
	background->SetPosition(Vector2(BACKGROUND_WIDTH, BACKGROUND_HEIGHT));
	SpriteComponent* backgroundsc = new SpriteComponent(background, 50);
	backgroundsc->SetTexture(GetTexture("Assets/Background.png"));

	// read file and get info
	std::ifstream file("Assets/Level1.txt");
	if (!file.is_open())
	{
		SDL_Log("Can't open file");
		return;
	}

	// start to iterate through all and get components inside
	std::string fileline;
	int ypos = 16;
	while (std::getline(file, fileline))
	{
		for (int col = 0; col < fileline.size(); ++col)
		{
			char element = fileline[col];
			Block* block = nullptr;
			Vector2 position(16 + col * 32, ypos);

			switch (element)
			{
			case '.':
				// just do nothing
				break;
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			{
				block = new Block(this, element);
				block->SetPosition(position);
				break;
			}
			case 'P':
				mPlayer = new Player(this);
				mPlayer->SetPosition(position);
				// Texture and collision set in player.cpp
				break;
			case 'Y':
				// Make Goomba
				Spawner* spawner = new Spawner(this);
				spawner->SetPosition(position);
				break;
			}
		}
		ypos += 32; // Move to the next row
	}

	Mix_Chunk* music = GetSound("Assets/Sounds/Music.ogg");
	mMusicChannel = Mix_PlayChannel(-1, music, -1);
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		Actor* actor = mActors.back();
		mActors.pop_back();
		delete actor;
	}

	for (auto& correspondTexture : mTexturePrototype) // for all things in texture prototype
	{
		SDL_DestroyTexture(correspondTexture.second); // in every map, destroy the texture
	}
	mTexturePrototype.clear(); //clear the map

	for (auto& correspondSound : mSoundPrototype)
	{
		Mix_FreeChunk(correspondSound.second); // release all sound
	}
	mSoundPrototype.clear();
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

void Game::AddBlock(Block* block)
{
	mBlock.push_back(block);
}

void Game::RemoveBlock(Block* block)
{
	auto iter = std::find(mBlock.begin(), mBlock.end(), block);
	if (iter != mBlock.end())
	{
		mBlock.erase(iter);
	}
}

void Game::AddGoomba(Goomba* goomba)
{
	mGoomba.push_back(goomba);
}

void Game::RemoveGoomba(Goomba* goomba)
{
	auto iter = std::find(mGoomba.begin(), mGoomba.end(), goomba);
	if (iter != mGoomba.end())
	{
		mGoomba.erase(iter);
	}
}

//Gettting sound:
Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	// similar to GetTexture, follow the style of mapping:
	auto iter = mSoundPrototype.find(fileName);

	if (iter != mSoundPrototype.end())
	{
		return iter->second; // return wanted sound
	}

	// to store the incoming sound
	Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());

	//if no sound?
	if (!sound)
	{
		SDL_Log("Can't load sound, %s, error: %s", fileName.c_str(), Mix_GetError());
		return nullptr;
	}

	// map
	mSoundPrototype[fileName] = sound;
	return sound;
}
