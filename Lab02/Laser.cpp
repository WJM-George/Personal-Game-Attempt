#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Laser.h"

Laser::Laser(Game* game)
: Actor(game)
, mMoveComponent(nullptr)
, mSpriteComponent(nullptr)
, mLifeTime(0.0f)
{
	mMoveComponent = new MoveComponent(this);
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Laser.png")); // for initial state
	mMoveComponent->SetForwardSpeed(mLaserSpeed);
}
//
void Laser::OnUpdate(float deltaTime)
{
	mLifeTime += deltaTime;
	if (mLifeTime >= 1.0f)
	{
		SetState(ActorState::Destroy); // in each initialization, will set to 0.0f as above
	}

	// to get a asteroid, maybe make use of getComponent function in actor.h?
	const std::vector<Asteroid*>& asteroids = mGame->GetAsteroid(); // this is crazy
	for (Asteroid* asteroid : asteroids) // iterate through all asteriods create in loaddata
	{
		float dist = Vector2::Distance(GetPosition(), asteroid->GetPosition());
		if (dist <= 70.0f)
		{
			SetState(ActorState::Destroy); // in laser
			asteroid->SetState(ActorState::Destroy);
			break;
		}
	}
}