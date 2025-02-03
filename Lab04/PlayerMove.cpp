#include "PlayerMove.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Block.h"
#include "Goomba.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
, mForwardSpeed(0.0f)
, mVerticalSpeed(0.0f)
, mInAir(false)
, mSpaceClick(false)
{
}
//
void PlayerMove::Update(float deltaTime)
{
	Game* game = mOwner->GetGame();
	CollisionComponent* playerColli = mOwner->GetComponent<CollisionComponent>();
	Vector2 pos = mOwner->GetPosition();
	pos.x += mForwardSpeed * deltaTime;

	//update vertical pos
	pos.y += mVerticalSpeed * deltaTime;

	// check fallen
	if (pos.y > FALLEN_FINISH_END)
	{
		mVerticalSpeed = 0.0f;
		mInAir = false;
		mOwner->SetState(ActorState::Paused);
		Mix_HaltChannel(game->mMusicChannel);
		Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/Dead.wav"), 0);
		AnimationUpdate();
		return;
	}
	//check win
	if (pos.x > WIN_SIDE)
	{
		mOwner->SetState(ActorState::Paused);
		// use halt to cut sound
		Mix_HaltChannel(game->mMusicChannel);
		Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/StageClear.wav"), 0);
		return;
	}

	mVerticalSpeed += GRAVITY * deltaTime;

	//at first in the air
	mInAir = true;

	//check colli
	for (auto block : game->GetBlock())
	{
		CollisionComponent* blockColli = block->GetComponent<CollisionComponent>();
		Vector2 offset;
		CollSide side = playerColli->GetMinOverlap(blockColli, offset);

		// change pos base collision
		mOwner->SetPosition(pos);

		if (side != CollSide::None)
		{
			// Separate logic for horizontal and vertical collisions
			if (side == CollSide::Top && mVerticalSpeed > 0.0f)
			{
				// fix vertical pos when falling on top of block
				pos.y += offset.y; // only vertical offset
				mVerticalSpeed = 0.0f;
				mInAir = false;
				AnimationUpdate(); // corresponding anime
			}
			else if (side == CollSide::Bottom && mVerticalSpeed < 0.0f)
			{
				pos.y += offset.y; // only vertical offset
				mVerticalSpeed = 0.0f;
				AnimationUpdate();
				Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/Bump.wav"), 0);
			}
			else if (side == CollSide::Left || side == CollSide::Right)
			{
				// fix horizontal position -> side collisions
				pos.x += offset.x; // only horizontal offset
				mForwardSpeed = 0.0f;
			}
		}
	}

	//
	//

	// check goomba colli
	for (Goomba* goomba : game->GetGoomba())
	{
		// just in case for the extreme case:
		// previously because the unused deletion cause some problem.
		if (goomba == nullptr || goomba->IsStomp() || goomba->GetState() == ActorState::Destroy)
		{
			continue;
		}
		CollisionComponent* goombaColli = goomba->GetComponent<CollisionComponent>();
		Vector2 offset;
		CollSide side = playerColli->GetMinOverlap(goombaColli, offset);

		if (side != CollSide::None)
		{
			// collision checking is ok
			if (side == CollSide::Top ||
				((side == CollSide::Left || side == CollSide::Right) && mInAir))
			{
				//if stomp
				goomba->SetStomp(true);
				// half jmp
				mVerticalSpeed = -JUMP_SPEED / 2;
				mInAir = true;
				AnimationUpdate();
				Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/Stomp.wav"), 0);
			}
			// below check mario finish?
			else
			{
				mOwner->SetState(ActorState::Paused);
				Mix_HaltChannel(game->mMusicChannel);
				Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/Dead.wav"), 0);
				AnimationUpdate();
			}
		}
	}
	//
	//

	mOwner->SetPosition(pos);

	// setting camera pos:
	Vector2& cameraPosition = game->GetCameraPos();
	float currentCameraX = mOwner->GetPosition().x - 300;

	if (pos.x < cameraPosition.x)
	{
		pos.x = cameraPosition.x;
	}
	// camera not backward:
	if (cameraPosition.x < currentCameraX)
	{
		cameraPosition.x = currentCameraX;
	}
	// not appear left black background;
	if (cameraPosition.x < 0)
	{
		cameraPosition.x = 0;
	}
	mOwner->SetPosition(pos);
	AnimationUpdate();
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	Game* game = mOwner->GetGame();
	mForwardSpeed = 0.0f;
	if (keyState[SDL_SCANCODE_D])
	{
		mForwardSpeed += FORWARD_SPEED;
	}
	if (keyState[SDL_SCANCODE_A])
	{
		mForwardSpeed -= FORWARD_SPEED;
	}

	// check jump
	if (keyState[SDL_SCANCODE_SPACE])
	{
		if (!mSpaceClick && !mInAir)
		{
			mVerticalSpeed = -JUMP_SPEED;
			mInAir = true;
			// jump sound:
			Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/Jump.wav"), 0);
		}
		mSpaceClick = true;
	}
	else
	{
		mSpaceClick = false;
	}
}

void PlayerMove::AnimationUpdate()
{
	AnimatedSprite* animspricomp = mOwner->GetComponent<AnimatedSprite>();

	//just in case
	if (!animspricomp)
	{
		return; // No animated sprite found
	}

	if (mOwner->GetState() == ActorState::Paused)
	{
		animspricomp->SetAnimation("dead");
		return;
	}

	// checking left right air condition below:
	if (!mInAir)
	{
		if (mForwardSpeed > 0.0f)
		{
			animspricomp->SetAnimation("runRight");
		}
		else if (mForwardSpeed < 0.0f)
		{
			animspricomp->SetAnimation("runLeft");
		}
		else
		{
			animspricomp->SetAnimation("idle");
		}
	}
	else
	{
		if (mForwardSpeed > 0.0f)
		{
			animspricomp->SetAnimation("jumpRight");
		}
		else if (mForwardSpeed < 0.0f)
		{
			animspricomp->SetAnimation("jumpLeft");
		}
		else
		{
			std::string currentAnim = animspricomp->GetAnimName();
			// just in case. also mario always go forward so by default should be heading right.
			if (currentAnim == "runRight" || currentAnim == "jumpRight" || currentAnim == "idle")
			{
				animspricomp->SetAnimation("jumpRight");
			}
			else
			{
				animspricomp->SetAnimation("jumpLeft");
			}
		}
	}
}
