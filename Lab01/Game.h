#pragma once

// TODO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <iostream>

class Game
{
public:
	Game() noexcept;
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	SDL_Window* mWindow;	 // window using SDL_CreateWindow
	SDL_Renderer* mRenderer; // renderer using SDL_CreateRenderer
	SDL_Point mPaddlePos;
	SDL_Point mBallPos;
	SDL_Point mBallVelocity;
	bool mLoopCondition;
	//
	//
	//// ___________________________________________________________________________
	//
	//
	// The following should all be const(want to set them const at first)
	// window setting
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;
	// wall
	const int VERTICAL_THICK = 15;
	const int HORIZONTAL_THICK = 15;
	// paddle and ball size
	const int PADDLE_THICK = 12;
	const int BALL_SIZE = 10;
	const int PADDLE_HEIGHT = 100;
	const int PADDLE_XDIS = 15;
	const float PADDLE_SPEED = 500.0f;
	// ball pos start right at the middle
	//
	// _______________________________________________________________________
	//
	// Delta time and Movement (for Ball and Paddle)
	// dt time
	Uint32 mPrevTime; // time from prev frm
	// paddle direction and speed
	float mPaddleMov;
	// ball direction and speed
	float mBallXVelo;
	float mBallYVelo;
	//
	//
	// _______________________________________________________________________
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
};

// construc the function in game.cpp in game.h order
