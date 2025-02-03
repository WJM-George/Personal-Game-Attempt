//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

// TODO
//
Game::Game() noexcept
: mWindow(nullptr)
, mRenderer(nullptr)
, mPaddlePos{0, 0}
, mBallPos{0, 0}
, mBallVelocity{0, 0}
, mLoopCondition(true)
, mPrevTime(0)
, mPaddleMov(0.0f)
, mBallXVelo(0.0f)
, mBallYVelo(0.0f)
{
}

//real constructor?
bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) //2 subsystem.
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	mWindow = SDL_CreateWindow("Game: Lab 1 Pong Ball", SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (!mWindow) //check window
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// create SDL render // with third para
	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// check again
	if (!mRenderer)
	{
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// paddle pos init
	mPaddlePos.x = PADDLE_XDIS;
	mPaddlePos.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;

	// ball pos init
	mBallPos.x = WINDOW_WIDTH / 2;
	mBallPos.y = WINDOW_HEIGHT / 2;

	// ball velocity init setting.
	mBallXVelo = 300; // I can't not figure out another way
	mBallYVelo = 300; // Is this regarded as a magic number?
	mBallVelocity.x = static_cast<int>(mBallXVelo);
	mBallVelocity.y = static_cast<int>(mBallYVelo);

	return true; // finally good to go
}

//real destructor??
void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

// break the game process by parts:
void Game::RunLoop()
{
	// Game loop belowww (infinite)
	while (mLoopCondition)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

bool Game::InitSDL(Uint32 flags)
{
	return SDL_Init(flags) == 0;
}

SDL_Window* Game::CreateSDLWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
{
	return SDL_CreateWindow(title, x, y, w, h, flags);
}

SDL_Renderer* Game::CreateSDLRenderer(SDL_Window* sdlWindow, int index, Uint32 flags)
{
	return SDL_CreateRenderer(sdlWindow, index, flags);
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{ // poll until all events are handled!
	  // decide what to do with this event.
		if (event.type == SDL_QUIT)
		{
			mLoopCondition = false; // 'X' button
		}
	}

	const Uint8* state =
		SDL_GetKeyboardState(NULL); // can be the representative of all keyboard action

	// Up or Down
	mPaddleMov = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleMov -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleMov += 1;
	}

	if (state[SDL_SCANCODE_ESCAPE]) // ESC for exit
	{
		mLoopCondition = false;
	}
}

void Game::GenerateOutput()
{
	// scene of the game
	// render color to blue
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	// clear buffer
	SDL_RenderClear(mRenderer);

	// pong ball and wall and paddle
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // white one

	// top
	SDL_Rect topWall{0, 0, WINDOW_WIDTH, VERTICAL_THICK};
	SDL_RenderFillRect(mRenderer, &topWall);

	// bottom
	SDL_Rect bottomWall{0, WINDOW_HEIGHT - VERTICAL_THICK, WINDOW_WIDTH, VERTICAL_THICK};
	SDL_RenderFillRect(mRenderer, &bottomWall);

	// right
	SDL_Rect rightWall{WINDOW_WIDTH - HORIZONTAL_THICK, 0, HORIZONTAL_THICK, WINDOW_HEIGHT};
	SDL_RenderFillRect(mRenderer, &rightWall);

	// draw paddle below
	SDL_Rect paddle{mPaddlePos.x - PADDLE_THICK / 2, // Centered X
					mPaddlePos.y,					 // Centered Y
					PADDLE_THICK, PADDLE_HEIGHT};
	SDL_RenderFillRect(mRenderer, &paddle);

	// draw ball below
	SDL_Rect ball{mBallPos.x - PADDLE_THICK / 2, // Centered X
				  mBallPos.y - PADDLE_THICK / 2, // Centered Y
				  BALL_SIZE, BALL_SIZE};
	SDL_RenderFillRect(mRenderer, &ball);

	// present all
	SDL_RenderPresent(mRenderer);
}

void Game::UpdateGame()
{
	// game overall logic?
	//1. Frame limit
	// Frame limit
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

	//4. Update all game objects by delta time

	if (mPaddleMov != 0)
	{
		mPaddlePos.y += static_cast<int>(mPaddleMov * deltaTime * PADDLE_SPEED);
	}
	// start of paddle at top
	if (mPaddlePos.y < VERTICAL_THICK)
	{
		mPaddlePos.y = VERTICAL_THICK;
	}
	else if (mPaddlePos.y > (WINDOW_HEIGHT - PADDLE_HEIGHT - VERTICAL_THICK))
	{
		mPaddlePos.y = WINDOW_HEIGHT - PADDLE_HEIGHT - VERTICAL_THICK;
	}

	// Ball conditions
	// 1. Ball's position by velo
	mBallPos.x += static_cast<int>(mBallVelocity.x * deltaTime);
	mBallPos.y += static_cast<int>(mBallVelocity.y * deltaTime);

	// 2. Ball bouncing
	// a. if paddle can't catch ball?

	if (mBallPos.x <= 0)
	{
		mLoopCondition = false;
	}

	// need to minus BallSize // initial set of speed if >0 -> south-eastern direc.
	//  So two < 0.0f, two > 0.0f
	// b. for top and bot
	if (mBallPos.y <= VERTICAL_THICK && mBallVelocity.y < 0.0f)
	{
		mBallVelocity.y = -mBallVelocity.y;
		mBallPos.y = VERTICAL_THICK;
	}
	else if (mBallPos.y >= (WINDOW_HEIGHT - VERTICAL_THICK - BALL_SIZE) && mBallVelocity.y > 0.0f)
	{
		mBallVelocity.y = -mBallVelocity.y;
		mBallPos.y = WINDOW_HEIGHT - VERTICAL_THICK - BALL_SIZE;
	}

	// c. for right
	if (mBallPos.x >= (WINDOW_WIDTH - HORIZONTAL_THICK - BALL_SIZE) && mBallVelocity.x > 0.0f)
	{
		mBallVelocity.x = -mBallVelocity.x;
		mBallPos.x = WINDOW_WIDTH - HORIZONTAL_THICK - BALL_SIZE;
	}

	// d. paddle catch the ball?
	if (mBallPos.x <= (PADDLE_XDIS + PADDLE_THICK) && mBallVelocity.x < 0.0f)
	{
		if (mBallPos.y >= mPaddlePos.y && mBallPos.y <= (mPaddlePos.y + PADDLE_HEIGHT))
		{
			mBallVelocity.x = -mBallVelocity.x;
			mBallPos.x = PADDLE_XDIS + PADDLE_THICK;
		}
	}
}
