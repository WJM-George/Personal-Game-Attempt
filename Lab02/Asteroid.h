#pragma once
#include "Actor.h"
#include "MoveComponent.h"

class Asteroid : public Actor
{
public:
	Asteroid(Game* game);
	void OnUpdate(float deltaTime) override;
	~Asteroid() override;

private:
	class MoveComponent* mMoveComponent;
	class SpriteComponent* mSpriteComponent;
	float mAsteroidSpeed = 150.0f;
};