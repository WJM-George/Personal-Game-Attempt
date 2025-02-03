#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder)
: mOwner(owner)
, mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this); // how it works? // when construct component,
								// no need to manually call AddComponent
}

Component::~Component()
{
}

void Component::Update(float deltaTime)
{
}

void Component::ProcessInput(const Uint8* keyState)
{
}

class Game* Component::GetGame() const
{
	return mOwner->GetGame();
}
