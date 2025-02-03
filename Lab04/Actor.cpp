#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector2::Zero)
, mScale(1.0f)
, mRotation(0.0f)
{
	mGame->AddActor(this); //Actor has mGame member var, then we use '''this'''
}

Actor::~Actor()
{
	mGame->RemoveActor(this); // similar to above

	for (Component* component : mComponents) // for all in mComponents
	{
		delete component;
	}

	mComponents.clear(); //remove things in std::vector
}

// returns forward direction vec of actor.
Vector2 Actor::GetForward() const
{
	return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); // return direc
																 // unit circle equation
}

void Actor::Update(float deltaTime)
{
	if (GetState() == ActorState::Active)
	{
		for (auto component : mComponents) // Loop over all components
		{
			component->Update(deltaTime);
		}
	}
	OnUpdate(deltaTime); //overridable by subclasses
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	if (GetState() == ActorState::Active)
	{
		for (auto component : mComponents) // same as above
		{
			component->ProcessInput(keyState);
		}
	}
	OnProcessInput(keyState); //overridable by subclasses
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
