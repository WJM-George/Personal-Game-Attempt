#include "Asteroid.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Random.h"

Asteroid::Asteroid(Game* game)
: Actor(game)
, mMoveComponent(nullptr)
, mSpriteComponent(nullptr)
{
	mGame->AddAsteroid(this); //Asteroid constructor

	mMoveComponent = new MoveComponent(this);
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Asteroid.png")); // for the asteroids
	mMoveComponent->SetForwardSpeed(mAsteroidSpeed);

	mRotation = Random::GetFloatRange(0.0f, Math::TwoPi); // for rotation

	Vector2 min(0.0f, 0.0f);
	Vector2 max(static_cast<float>(mGame->GetWindowWidth()),
				static_cast<float>(game->GetWindowHeight()));
	mPosition = Random::GetVector(min, max);
}

Asteroid::~Asteroid()
{
	mGame->RemoveAsteroid(this);
}

void Asteroid::OnUpdate(float deltaTime)
{
	if (mPosition.x >= static_cast<float>(mGame->GetWindowWidth()))
	{
		mPosition.x = 0.0f;
	}
	else if (mPosition.x < 0.0f)
	{
		mPosition.x = static_cast<float>(mGame->GetWindowWidth());
	}

	if (mPosition.y >= static_cast<float>(mGame->GetWindowHeight()))
	{
		mPosition.y = 0.0f;
	}
	else if (mPosition.y < 0.0f)
	{
		mPosition.y = static_cast<float>(mGame->GetWindowHeight());
	}
}