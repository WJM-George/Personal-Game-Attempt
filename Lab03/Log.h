#pragma once
#include "Actor.h"

class Log : public Actor
{
public:
	enum class Type
	{
		X,
		Y,
		Z
	};
	Log(Game* game, Type type); // log sprite in game.cpp loaddata
	~Log() override;

private:
	class CollisionComponent* mCollisionComponent;
	const float LOG_SPEED = 37.5f;
};
