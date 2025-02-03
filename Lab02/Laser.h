#pragma once
#include "Actor.h"
#include "MoveComponent.h"
#include "Asteroid.h"
#include "Game.h"

class Laser : public Actor
{
public:
	Laser(Game* game);
	void OnUpdate(float deltaTime) override;

private:
	class MoveComponent* mMoveComponent;
	class SpriteComponent* mSpriteComponent;
	float mLifeTime;
	float mLaserSpeed = 400.0f;
};