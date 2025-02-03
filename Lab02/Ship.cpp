#include "Ship.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
: Actor(game)
, mMoveComponent(nullptr)
, mSpriteComponent(nullptr)
, mCoolDown(0.0f)
{
	mMoveComponent = new MoveComponent(this);
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Ship.png")); // for initial state
}

void Ship::OnProcessInput(const Uint8* keyState)
{
	float angularspeed = 0.0f;
	float linearspeed = 0.0f;

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_D])
	{
		angularspeed -= Math::Pi; // anti-clockwise
	}
	if (state[SDL_SCANCODE_A])
	{
		angularspeed += Math::Pi; // clockwise
	}

	if (state[SDL_SCANCODE_W])
	{
		linearspeed += mLinearSpeed;
	}
	if (state[SDL_SCANCODE_S])
	{
		linearspeed -= mLinearSpeed;
	}

	mMoveComponent->SetAngularSpeed(angularspeed); //set it after each operation
	mMoveComponent->SetForwardSpeed(linearspeed);

	if (linearspeed != 0.0f)
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	else
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
	}

	if (mCoolDown <= 0.0f && state[SDL_SCANCODE_SPACE])
	{
		Laser* laser = new Laser(mGame);
		laser->SetPosition(this->GetPosition());
		laser->SetRotation(this->GetRotation());
		mCoolDown = 1.0f;
	}
}

void Ship::OnUpdate(float deltaTime)
{
	mCoolDown -= deltaTime;
}
