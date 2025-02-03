#pragma once
#include "Actor.h"
#include "WrappingMove.h"

class Vehicle : public Actor
{
public:
	enum class Type // inspire by CollisionComponent.h
	{
		A,
		B,
		C,
		D,
		T
	};

	Vehicle(Game* game, Type type); // vehicle sprite in game.cpp loaddata
	~Vehicle() override;

	void OnUpdate(float deltaTime) override;

protected: // for collision and slow down
	class CollisionComponent* mCollisionComponent;
	WrappingMove* mWrappingMove;
	const float VEHICLE_SPEED = 37.5f;
};