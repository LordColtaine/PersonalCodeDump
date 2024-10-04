#pragma once


#include "Pose.h"
#include "math\mat4.h"
#include <vector>
#include <string>

class Skeleton {
protected:
	Pose mBindPose;
	std::vector<Transform> mInvBindPose;
	std::vector<std::string> mJointNames;
protected:
	void UpdateInverseBindPose();
public:
	Skeleton();
	Skeleton(const Pose bind, const std::vector<std::string>& names);

	void Set( const Pose bind, const std::vector<std::string>& names);
	Pose GetBindPoseValues();
	Pose& GetBindPose();
	std::vector<Transform>& GetInvBindPose();
	std::vector<std::string>& GetJointNames();
	std::string& GetJointName(unsigned int index);

	void SetTransforms(std::vector<Transform> inpTransforms)
	{
		mInvBindPose = inpTransforms;
	}

	Transform GetInvTransform(int index)
	{
		return mInvBindPose[index];
	}
};

