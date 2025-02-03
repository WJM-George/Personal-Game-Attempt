#include "Goomba.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "GoombaMove.h"

Goomba::Goomba(Game* game)
: Actor(game)
, mCollisionComponent(new CollisionComponent(this))
, mAnimatedSprite(new AnimatedSprite(this, 150))
, mGoombaMove(new GoombaMove(this))
, mStomp(false)
, mLifeTimeGoomba(0.0f)
{

	mCollisionComponent->SetSize(32.0f, 32.0f);
	// for alternating the behavior
	// clever way to store all the actions of a character
	// use vector to store all the animations
	// update by frame and change it to corresponding state
	std::vector<SDL_Texture*> walkAnim{GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
									   GetGame()->GetTexture("Assets/Goomba/Walk1.png")};

	mAnimatedSprite->AddAnimation("walk", walkAnim);

	// and also for anime of death:
	std::vector<SDL_Texture*> deadAnim{GetGame()->GetTexture("Assets/Goomba/Dead.png")};
	mAnimatedSprite->AddAnimation("dead", deadAnim);

	//init: should walk
	mAnimatedSprite->SetAnimation("walk");
	game->AddGoomba(this);
}

Goomba::~Goomba()
{
	mGame->RemoveGoomba(this);
}

void Goomba::SetStomp(bool stomp)
{
	mStomp = stomp;
	if (stomp)
	{
		mAnimatedSprite->SetAnimation("dead");
		mGoombaMove->SetForwardSpeed(0.0f);
		mLifeTimeGoomba = 0.25f;
	}
}

void Goomba::UpdateGoomba(float deltaTime)
{
	if (mStomp)
	{
		if (mLifeTimeGoomba > 0.0f)
		{
			mLifeTimeGoomba -= deltaTime;
			if (mLifeTimeGoomba <= 0.0f)
			{
				mGame->RemoveGoomba(this);
				SetState(ActorState::Destroy);
			}
		}
	}

	if (GetPosition().y > 448.0f)
	{
		// revise the fallen goomba problem here:
		mGoombaMove->SetForwardSpeed(0.0f);
		mGame->RemoveGoomba(this);
		SetState(ActorState::Destroy); // destroy it
	}
}
