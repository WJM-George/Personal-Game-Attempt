#pragma once
#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(Game* game);
	class MoveComponent* GetMoveComponent() const { return mMoveComponent; } // similar to template

	// override OnProcessInput
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;

private:
	class MoveComponent* mMoveComponent;
	class SpriteComponent* mSpriteComponent;
	float mCoolDown;
	float mLinearSpeed = 250.0f;
};