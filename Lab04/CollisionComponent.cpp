#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	Vector2 minA = GetMin();
	Vector2 maxA = GetMax();
	Vector2 minB = other->GetMin();
	Vector2 maxB = other->GetMax();
	bool intersection = (minB.x > maxA.x || minA.x > maxB.x || minA.y > maxB.y || minB.y > maxA.y);
	return (!intersection);
}

Vector2 CollisionComponent::GetMin() const
{
	Vector2 positionOwner = mOwner->GetPosition();
	float scaleOwner = mOwner->GetScale();
	Vector2 min;
	min.x = positionOwner.x - (mWidth * scaleOwner) / 2.0f; // graph for understanding
	min.y = positionOwner.y - (mHeight * scaleOwner) / 2.0f;
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	Vector2 positionOwner = mOwner->GetPosition();
	float scaleOwner = mOwner->GetScale();
	Vector2 max;
	max.x = positionOwner.x + (mWidth * scaleOwner) / 2.0f;
	max.y = positionOwner.y + (mHeight * scaleOwner) / 2.0f;
	return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector2& offset) const
{
	// similarly we get pos of each component;
	Vector2 minA = GetMin();
	Vector2 maxA = GetMax();
	Vector2 minB = other->GetMin();
	Vector2 maxB = other->GetMax();

	// compute with graph assist // always other minus this
	float leftDist = minB.x - maxA.x;
	float rightDist = maxB.x - minA.x;
	float topDist = minB.y - maxA.y;
	float botDist = maxB.y - minA.y;

	// pick a min dist one
	float minOverlap =
		std::min({std::abs(leftDist), std::abs(rightDist), std::abs(topDist), std::abs(botDist)});

	// check if intersect first // below are all the extreme cases.
	if (leftDist > 0 || rightDist < 0 || topDist > 0 || botDist < 0)
	{
		offset = Vector2(0.0f, 0.0f);
		return CollSide::None;
	}

	if (minOverlap == std::abs(leftDist))
	{
		offset = Vector2(leftDist, 0.0f);
		return CollSide::Left;
	}
	else if (minOverlap == std::abs(rightDist))
	{
		offset = Vector2(rightDist, 0.0f);
		return CollSide::Right;
	}
	else if (minOverlap == std::abs(topDist))
	{
		offset = Vector2(0.0f, topDist);
		return CollSide::Top;
	}
	else if (minOverlap == std::abs(botDist))
	{
		offset = Vector2(0.0f, botDist);
		return CollSide::Bottom;
	}

	return CollSide::None;
}
