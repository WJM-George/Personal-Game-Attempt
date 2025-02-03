#pragma once
#include "MoveComponent.h"
#include "Actor.h"

class GoombaMove : public MoveComponent
{
public:
	GoombaMove(class Actor* owner);
	void Update(float deltaTime) override;

private:
	float mForwardSpeed;
	float mVerticalSpeed;
	const float GRAVITY = 2000.0f;
};