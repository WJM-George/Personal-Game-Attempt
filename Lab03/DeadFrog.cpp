#include "DeadFrog.h"
#include "Game.h"
#include "SpriteComponent.h"

DeadFrog::DeadFrog(Game* game)
: Actor(game)
, mLifeTime(0.5f)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(game->GetTexture("Assets/Dead.png"));
}

void DeadFrog::OnUpdate(float deltaTime)
{
	mLifeTime -= deltaTime; // like lasers
	if (mLifeTime <= 0.0f)
	{
		SetState(ActorState::Destroy); // after 0.5f set it to gone.
	}
}
