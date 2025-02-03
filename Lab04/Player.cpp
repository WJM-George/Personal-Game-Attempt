#include "Player.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PlayerMove.h"

Player::Player(Game* game)
: Actor(game)
, mCollisionComponent(new CollisionComponent(this))
, mAnimatedSprite(new AnimatedSprite(this, 150))
, mPlayerMove(new PlayerMove(this))
{
	//AnimatedSprite* playersc = new AnimatedSprite(this);
	mCollisionComponent->SetSize(32.0f, 32.0f);

	// setitng for all possible anim
	std::vector<SDL_Texture*> idleAnim{GetGame()->GetTexture("Assets/Mario/Idle.png")};

	std::vector<SDL_Texture*> deadAnim{GetGame()->GetTexture("Assets/Mario/Dead.png")};

	std::vector<SDL_Texture*> jumpLeftAnim{GetGame()->GetTexture("Assets/Mario/JumpLeft.png")};

	std::vector<SDL_Texture*> jumpRightAnim{GetGame()->GetTexture("Assets/Mario/JumpRight.png")};

	std::vector<SDL_Texture*> runLeftAnim{GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
										  GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
										  GetGame()->GetTexture("Assets/Mario/RunLeft2.png")};

	std::vector<SDL_Texture*> runRightAnim{GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
										   GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
										   GetGame()->GetTexture("Assets/Mario/RunRight2.png")};

	mAnimatedSprite->AddAnimation("idle", idleAnim);
	mAnimatedSprite->AddAnimation("dead", deadAnim);
	mAnimatedSprite->AddAnimation("jumpLeft", jumpLeftAnim);
	mAnimatedSprite->AddAnimation("jumpRight", jumpRightAnim);
	mAnimatedSprite->AddAnimation("runLeft", runLeftAnim);
	mAnimatedSprite->AddAnimation("runRight", runRightAnim);

	mAnimatedSprite->SetAnimation("idle");
}

Player::~Player()
{
}