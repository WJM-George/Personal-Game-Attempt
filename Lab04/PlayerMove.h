#pragma once
#include "MoveComponent.h"
#include "Actor.h"

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Actor* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void AnimationUpdate();

private:
	float mForwardSpeed;
	float mVerticalSpeed;
	const float GRAVITY = 2000.0f;
	bool mInAir;
	bool mSpaceClick;
	const float WIN_SIDE = 6368.0f;
	const float FALLEN_FINISH_END = 520.0f;
	const float FORWARD_SPEED = 300.0f;
	const float JUMP_SPEED = 700.0f;
};
