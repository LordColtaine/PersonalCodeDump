#pragma once
#include "Keyframe.h"

class JointAnimation
{
public:
	JointAnimation() = default;
	JointAnimation(unsigned int index, std::vector<KeyFrame> keyframes)
		:
		index(index),
		keyframes(keyframes)
	{}
	Transform GetCurrentTransform(float t) const
	{
		std::vector<KeyFrame> kfs = GetRelevantTransforms(t);
		float progress = GetProgression(t, kfs[0], kfs[1]);
		Transform final = mix(kfs[0].GetTransform(), kfs[1].GetTransform(), progress);
		return final;
	}

	float GetProgression(const float t, const KeyFrame before, const KeyFrame after) const
	{
		float tot = after.GetTimeStamp() - before.GetTimeStamp();
		if (0 == tot) return 0;
		float cur = t - before.GetTimeStamp();
		return cur / tot;
	}

	std::vector<KeyFrame> GetRelevantTransforms(float t) const
	{
		KeyFrame before = keyframes[0];
		KeyFrame after = keyframes[0];
		int index0, index1;
		index0 = index1 = 0;
		for (unsigned int i = 1; i < keyframes.size(); i++)
		{
			after = keyframes[i], index1 = i;
			if (keyframes[i].GetTimeStamp() > t)
			{
				break;
			}
			before = keyframes[i], index0 = i;
		}
		
		std::vector<KeyFrame> kfs = { before, after };
		return kfs;
	}
	unsigned int GetIndex() const
	{
		return index;
	}
private:
	unsigned int index = -1;
	std::vector<KeyFrame> keyframes;
};