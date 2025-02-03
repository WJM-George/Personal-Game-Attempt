#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (mAnimName.empty())
	{
		return;
	}

	// store animation
	const auto& frameAnims = mAnims[mAnimName];
	// get the current frame count;
	// no need to times other things, since we consider 1 sec each update
	size_t countingFrame = frameAnims.size();

	// if animation no frame, handle edge
	if (countingFrame == 0)
	{
		return;
	}

	if (!mIsPaused)
	{
		mAnimTimer += mAnimFPS * deltaTime;

		if (countingFrame > 1)
		{
			if (mAnimTimer >= countingFrame)
			{
				mAnimTimer -= countingFrame;
			}
		}
		else
		{
			// if 1-frame anime, no need to wrap at all
			mAnimTimer = 0.0f;
		}
	}
	// get the current frame info base on timer
	size_t currentFrame = static_cast<size_t>(mAnimTimer);
	SetTexture(frameAnims[currentFrame]); // using current frame to set the animation
}

void AnimatedSprite::AddAnimation(const std::string& name, const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
