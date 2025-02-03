#pragma once
#include "MoveComponent.h"
#include "Math.h"
class WrappingMove : public MoveComponent
{
public:
	WrappingMove(class Actor* owner);
	void Update(float deltaTime) override;
	void SetDirection(const Vector2& direction) { mDirec = direction; }
	Vector2 GetDirection() const; // for slow down task

private:
	Vector2 mDirec;
};