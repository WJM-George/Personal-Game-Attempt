#include "GoombaMove.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Block.h"
#include "Goomba.h"

GoombaMove::GoombaMove(Actor* owner)
: MoveComponent(owner)
, mForwardSpeed(-100.0f) // goomba speed
, mVerticalSpeed(0.0f)
{
}

void GoombaMove::Update(float deltaTime)
{
	Game* game = mOwner->GetGame();
	Goomba* goomba = static_cast<Goomba*>(mOwner);
	CollisionComponent* goombaColli = mOwner->GetComponent<CollisionComponent>();
	//
	//
	// set for destroy the goomba
	// crazy implementation
	goomba->UpdateGoomba(deltaTime);
	// set for 0-rize the whole process of a certain goomba
	if (goomba->IsStomp())
	{
		// if stomp, just stop.
		return;
	}

	Vector2 pos = mOwner->GetPosition();
	pos.x += mForwardSpeed * deltaTime;
	pos.y += mVerticalSpeed * deltaTime;

	mVerticalSpeed += GRAVITY * deltaTime;

	// what we need to do here is to check two things:
	// colli with other goomba? colli with other block?

	// First check block: left? right? top? (no bottom scenario)
	for (Block* block : game->GetBlock())
	{
		CollisionComponent* blockColli = block->GetComponent<CollisionComponent>();
		Vector2 offset;
		CollSide side = goombaColli->GetMinOverlap(blockColli, offset);

		pos += offset;
		mOwner->SetPosition(pos);

		if (side == CollSide::Top && mVerticalSpeed > 0.0f)
		{
			mVerticalSpeed = 0.0f;
		}
		else if (side == CollSide::Left || side == CollSide::Right)
		{
			mForwardSpeed = -mForwardSpeed;
		}
	}

	for (Goomba* otherGoomba : game->GetGoomba())
	{
		// avoid self collision
		if (otherGoomba != mOwner)
		{
			CollisionComponent* otherGoombaColli = otherGoomba->GetComponent<CollisionComponent>();
			Vector2 offset;
			CollSide side = goombaColli->GetMinOverlap(otherGoombaColli, offset);

			if (side != CollSide::None)
			{
				pos += offset;
				mOwner->SetPosition(pos);

				Vector2 otherGoombaPos = otherGoomba->GetPosition();
				GoombaMove* otherGoombaMove = otherGoomba->GetComponent<GoombaMove>();
				// very important below, or will end up crazy movement
				otherGoombaPos -= offset; // here should be subtract in order to collide
				otherGoomba->SetPosition(otherGoombaPos);

				// Update forward speed for both Goombas
				if (side == CollSide::Left || side == CollSide::Right)
				{
					mForwardSpeed = -mForwardSpeed;
					otherGoombaMove->SetForwardSpeed(-otherGoombaMove->GetForwardSpeed());
				}
			}
		}
	}

	if (pos.x < 0.0f)
	{
		pos.x = 0.0f; // not go out of screen
	}

	mOwner->SetPosition(pos);
}