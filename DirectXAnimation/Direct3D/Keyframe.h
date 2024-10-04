#pragma once
#include "Pose.h"

class KeyFrame
{
public:
	KeyFrame() = default;
	KeyFrame(float timeStamp, Transform transform)
		:
		timeStamp(timeStamp),
		transform(transform)
	{}
	float GetTimeStamp() const
	{
		return timeStamp;
	}
	Transform& GetTransform()
	{
		return transform;
	}

private:
	float timeStamp;
	Transform transform;
};