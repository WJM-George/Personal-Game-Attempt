#include "Log.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "WrappingMove.h"
#include "Game.h"

Log::Log(Game* game, Type type)
: Actor(game)
, mCollisionComponent(new CollisionComponent(this))
{
	// Create a sprite component and attach it to the log
	WrappingMove* wm = new WrappingMove(this); // move component
	wm->SetForwardSpeed(LOG_SPEED);

	SpriteComponent* spco = new SpriteComponent(this);

	if (type == Type::X)
	{
		spco->SetTexture(game->GetTexture("Assets/LogX.png"));
		mCollisionComponent->SetSize(96.0f, 24.0f);
	}
	else if (type == Type::Y)
	{
		spco->SetTexture(game->GetTexture("Assets/LogY.png"));
		mCollisionComponent->SetSize(128.0f, 24.0f);
	}
	else if (type == Type::Z)
	{
		spco->SetTexture(game->GetTexture("Assets/LogZ.png"));
		mCollisionComponent->SetSize(192.0f, 24.0f);
	}

	game->AddLog(this);
}

Log::~Log()
{
	mGame->RemoveLog(this);
}
