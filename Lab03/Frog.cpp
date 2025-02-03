#include "Frog.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Vehicle.h"
#include "DeadFrog.h"
#include "Log.h"

Frog::Frog(Game* game)
: Actor(game)
, mCollisionComponent(new CollisionComponent(this))
{
	// frog always be the frog
	SpriteComponent* sc = new SpriteComponent(this, 150); // change draw order of frog
	sc->SetTexture(game->GetTexture("Assets/Frog.png"));
	mCollisionComponent->SetSize(FROG_SIZE, FROG_SIZE);

	// for simplicity
	//new
	mGoalColliComponent = game->GetGoalColliComp();
	mGoalPosition = game->GetGoal()->GetPosition();
}

void Frog::OnProcessInput(const Uint8* keyState)
{
	// if get to goal, then stop update
	if (GetState() == ActorState::Paused)
	{
		return;
	}

	// overall idea of updating: 4 conditions:
	// Click 'W', 'S', 'A', 'D'. Then update position
	// And use Math::Clamp to restrict the area
	// finally get the newest click to the keyboard state.

	Vector2 pos = GetPosition();

	if (keyState[SDL_SCANCODE_W] &&
		!mKeyBoardClick[SDL_SCANCODE_W]) // check click once and
										 // avoid holding and can still go forward
	{
		pos.y -= ONEJUMP; // refer to SDL coordinate
	}

	if (keyState[SDL_SCANCODE_S] && !mKeyBoardClick[SDL_SCANCODE_S])
	{

		pos.y += ONEJUMP;
	}

	if (keyState[SDL_SCANCODE_A] && !mKeyBoardClick[SDL_SCANCODE_A])
	{

		pos.x -= ONEJUMP;
	}

	if (keyState[SDL_SCANCODE_D] && !mKeyBoardClick[SDL_SCANCODE_D])
	{

		pos.x += ONEJUMP;
	}

	// restrict the clamp at this place
	pos.y = Math::Clamp(pos.y, FROG_START_PLACE,
						static_cast<float>(mGame->GetWindowHeight()) - CRASH_LIMIT);
	pos.x = Math::Clamp(pos.x, 0.0f, static_cast<float>(mGame->GetWindowWidth()));

	// update the position at the end
	SetPosition(pos);

	mKeyBoardClick[SDL_SCANCODE_W] = keyState[SDL_SCANCODE_W];
	mKeyBoardClick[SDL_SCANCODE_S] = keyState[SDL_SCANCODE_S];
	mKeyBoardClick[SDL_SCANCODE_A] = keyState[SDL_SCANCODE_A];
	mKeyBoardClick[SDL_SCANCODE_D] = keyState[SDL_SCANCODE_D];
}

void Frog::OnUpdate(float deltaTime)
{
	// to goal = stop updating
	if (GetState() == ActorState::Paused)
	{
		return;
	}

	mRideOnLog = false;
	for (Vehicle* vehicle : mGame->GetVehicle())
	{
		// new
		CollisionComponent* vehicleColli =
			vehicle->GetComponent<CollisionComponent>(); // local variable for
		// GetComponent<>, since vehicle is not a class for frog. can not be stored in a member variable.
		if (mCollisionComponent->Intersect(vehicleColli))
		{
			DeadFrog* deadFrog = new DeadFrog(GetGame());
			deadFrog->SetPosition(GetPosition());
			SetPosition(START_FROG); //go back?
			break;
		}
	}

	for (Log* log : mGame->GetLog())
	{
		Vector2 offset;
		// same as vehicleColli
		CollisionComponent* logColli = log->GetComponent<CollisionComponent>();
		// create a logWrapMove for revising the Extra GetComponent problem.
		WrappingMove* logWrapMove = log->GetComponent<WrappingMove>();
		// check whether ride on log or not:
		CollSide side = mCollisionComponent->GetMinOverlap(logColli, offset);

		if (side != CollSide::None)
		{
			mRideOnLog = true;
			// when touch, set frog pos to log pos, x: itself (for now), y: log.y
			SetPosition(Vector2(GetPosition().x, log->GetPosition().y));

			// the let the frog ride with the log:
			Vector2 direc = logWrapMove->GetDirection();
			float linearSpeed = logWrapMove->GetForwardSpeed();
			Vector2 moveMent = direc * linearSpeed * deltaTime;
			SetPosition(GetPosition() + moveMent); // let frog move with log

			// if collide left (frog be collide) (side is in log):
			if (side == CollSide::Left)
			{
				// right 16 to get on the log
				SetPosition(Vector2(GetPosition().x + offset.x + ON_LOG_EXTRA, GetPosition().y));
			}
			else if (side == CollSide::Right)
			{
				// similarly
				SetPosition(Vector2(GetPosition().x + offset.x - ON_LOG_EXTRA, GetPosition().y));
			}
		}
	}

	//check if frog reach goal
	if (mCollisionComponent->Intersect(mGoalColliComponent))
	{
		SetPosition(mGoalPosition);
		SetState(ActorState::Paused);
		return;
	}

	// if in water
	if (!mRideOnLog && (GetPosition().y >= WATER_UP_BOUND && GetPosition().y <= WATER_LOW_BOUND))
	{
		DeadFrog* deadFrog = new DeadFrog(GetGame());
		deadFrog->SetPosition(GetPosition());
		SetPosition(START_FROG); //go back?
	}

	// grassland should also be finished
	if (GetPosition().y < WATER_UP_BOUND && !mCollisionComponent->Intersect(mGoalColliComponent))
	{
		DeadFrog* deadFrog = new DeadFrog(GetGame());
		deadFrog->SetPosition(GetPosition());
		SetPosition(START_FROG); // go back to start
	}
}
