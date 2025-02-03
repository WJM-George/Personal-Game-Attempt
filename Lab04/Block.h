#pragma once
#include "Actor.h"

class Block : public Actor
{
public:
	Block(Game* game, char type);
	~Block() override;

private: // for colli and sprite
	class CollisionComponent* mCollisionComponent;
	class SpriteComponent* mSpriteComponent;
};