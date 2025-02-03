#include "Spawner.h"
#include "Game.h"
#include "Player.h"
#include "Goomba.h"

Spawner::Spawner(Game* game)
: Actor(game)
{
}
//
void Spawner::OnUpdate(float deltaTime)
{
	Vector2 marioPosition = mGame->GetPlayer()->GetPosition();

	if (GetPosition().x - marioPosition.x < 600.0f)
	{
		Goomba* goomba = new Goomba(mGame);
		goomba->SetPosition(GetPosition());
		mGame->AddGoomba(goomba);
		SetState(ActorState::Destroy);
	}
}
