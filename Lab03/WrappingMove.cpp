#include "WrappingMove.h"
#include "Actor.h"
#include "Game.h"

WrappingMove::WrappingMove(Actor* owner)
: MoveComponent(owner)
, mDirec(Vector2::Zero)
{
}

void WrappingMove::Update(float deltaTime)
{
	Vector2 pos = mOwner->GetPosition();
	pos += mDirec * mForwardSpeed * deltaTime;
	const int WINDOW_WIDTH_UPDATE = 448;

	if (pos.x < 0.0f)
	{
		pos.x = static_cast<float>(WINDOW_WIDTH_UPDATE);
	}
	else if (pos.x > static_cast<float>(WINDOW_WIDTH_UPDATE))
	{
		pos.x = 0.0f;
	}

	mOwner->SetPosition(pos);
}

Vector2 WrappingMove::GetDirection() const
{
	return mDirec;
}
