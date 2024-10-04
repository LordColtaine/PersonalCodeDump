#pragma once


#include "JointAnimation.h"
#include "Skeleton.h"
class Animation
{
public:
	Animation() = default;
	Animation(float time, std::vector<JointAnimation> jointAnims)
		:
		totalTime(time),
		jointAnims(jointAnims)
	{}
	float GetTotalTime()
	{
		return totalTime;
	}
	Pose GetCurPose(float t, Skeleton& skeleton)
	{
		if (0 == jointAnims.size())
		{
			return skeleton.GetBindPoseValues();
		}
		if (t > totalTime)
		{
			t = fmodf(t, totalTime);
		}
		Pose curPose = skeleton.GetBindPoseValues();
		for (auto& joint : jointAnims)
		{
			auto index = joint.GetIndex();
			auto tf = joint.GetCurrentTransform(t);
			curPose.SetLocalTransform(index, tf);
		}
		return curPose;
	}

private:
	
private:
	float totalTime;
	std::vector<JointAnimation> jointAnims;
};