#include "Block.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"

Block::Block(Game* game, char type)
: Actor(game)
, mCollisionComponent(new CollisionComponent(this))
, mSpriteComponent(new SpriteComponent(this))
{
	game->AddBlock(this);
	mCollisionComponent->SetSize(32.0f, 32.0f);

	std::string texturePath = "Assets/Block" + std::string(1, type) + ".png";
	mSpriteComponent->SetTexture(game->GetTexture(texturePath));
}

Block::~Block()
{
	mGame->RemoveBlock(this);
}
