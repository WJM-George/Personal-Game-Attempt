#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
: Component(owner, 50) // order is lower since set it 50.
, mAngularSpeed(0.0f)
, mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearlyZero(mAngularSpeed)) // if only when the speed is valid
	{
		float rotation = mOwner->GetRotation();
		rotation += mAngularSpeed * deltaTime; // angle in dt
		mOwner->SetRotation(rotation);
	}

	if (!Math::NearlyZero(mForwardSpeed))
	{
		Vector2 position = mOwner->GetPosition();
		position += mForwardSpeed * deltaTime * mOwner->GetForward(); // v*t*direc
		mOwner->SetPosition(position);
	}
}
