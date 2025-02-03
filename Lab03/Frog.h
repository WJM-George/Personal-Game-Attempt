#pragma once
#include "Actor.h"
#include "Game.h"

class Frog : public Actor
{
public:
	Frog(Game* game);
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;

private:
	std::unordered_map<SDL_Scancode, bool>
		mKeyBoardClick; //“last frame” values (use a bool), use a map to
						// match the last operation on keyboard.
	const Vector2 START_FROG = Vector2(224.0f, 464.0f);
	class CollisionComponent* mCollisionComponent; // a member for simplification.
	class CollisionComponent* mGoalColliComponent;
	Vector2 mGoalPosition;

	const float CRASH_LIMIT = 48.0f;
	const float FROG_START_PLACE = 80.0f;
	const float FROG_SIZE = 25.0f;
	const float ONEJUMP = 32.0f;		  // one jump dist //new
	const float WATER_UP_BOUND = 90.0f;	  //new
	const float WATER_LOW_BOUND = 255.0f; //new
	const float ON_LOG_EXTRA = 16.0f;	  //new
	bool mRideOnLog = false;
};
