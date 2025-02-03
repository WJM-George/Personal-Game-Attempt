#include "Vehicle.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Frog.h"
#include "Game.h"

Vehicle::Vehicle(Game* game, Type type)
: Actor(game)
, mCollisionComponent(new CollisionComponent(this)) // if not this way, compiler error
, mWrappingMove(new WrappingMove(this))
{
	// Create a sprite component and attach it to the vehicle
	WrappingMove* wm = new WrappingMove(this);
	wm->SetForwardSpeed(VEHICLE_SPEED);

	SpriteComponent* spco = new SpriteComponent(this);

	if (type == Type::A)
	{
		spco->SetTexture(game->GetTexture("Assets/CarA.png"));
		mCollisionComponent->SetSize(32.0f, 32.0f);
	}
	else if (type == Type::B)
	{
		spco->SetTexture(game->GetTexture("Assets/CarB.png"));
		mCollisionComponent->SetSize(32.0f, 32.0f);
	}
	else if (type == Type::C)
	{
		spco->SetTexture(game->GetTexture("Assets/CarC.png"));
		mCollisionComponent->SetSize(32.0f, 32.0f);
	}
	else if (type == Type::D)
	{
		spco->SetTexture(game->GetTexture("Assets/CarD.png"));
		mCollisionComponent->SetSize(32.0f, 32.0f);
	}

	else if (type == Type::T)
	{
		spco->SetTexture(game->GetTexture("Assets/Truck.png"));
		mCollisionComponent->SetSize(64.0f, 24.0f);
	}
	game->AddVehicle(this);
}

Vehicle::~Vehicle()
{
	mGame->RemoveVehicle(this);
}

void Vehicle::OnUpdate(float deltaTime)
{
	Vector2 vehiclePos = GetPosition();
	Vector2 frogPos = mGame->GetFrog()->GetPosition(); //get stuck// how to access frog?
	Vector2 vehicleTofrog = frogPos - vehiclePos;
	vehicleTofrog.Normalize(); // for further implementation

	Vector2 moveDirec = mWrappingMove->GetDirection();

	float dotProduct = Vector2::Dot(vehicleTofrog, moveDirec);
	float angle = Math::Acos(dotProduct);
	const float SET_ANGLE = Math::PiOver2 / 3.0f;

	if (angle <= SET_ANGLE)
	{
		mWrappingMove->SetForwardSpeed(VEHICLE_SPEED / 2);
	}
	else
	{
		mWrappingMove->SetForwardSpeed(VEHICLE_SPEED);
	}
}
