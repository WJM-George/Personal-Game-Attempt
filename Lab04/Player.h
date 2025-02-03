#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(Game* game); // vehicle sprite in game.cpp loaddata
	~Player() override;

	class CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }

private: // for colli and sprite
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mAnimatedSprite;
	class PlayerMove* mPlayerMove;
};