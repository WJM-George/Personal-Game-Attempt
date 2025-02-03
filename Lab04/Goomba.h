#pragma once
#include "Actor.h"

class Goomba : public Actor
{
public:
	Goomba(class Game* game);
	~Goomba() override;

	class CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
	bool IsStomp() const { return mStomp; }
	void SetStomp(bool stomp);
	void UpdateGoomba(float deltaTime);

private: // for colli and sprite
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mAnimatedSprite;
	class GoombaMove* mGoombaMove;
	bool mStomp;
	float mLifeTimeGoomba;
};