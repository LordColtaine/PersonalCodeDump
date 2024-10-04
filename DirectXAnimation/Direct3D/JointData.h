#pragma once
#include "math\Transform.h"
#include <vector>
class Joint
{
public:
	Joint() = default;
	Joint(unsigned int index, Transform transform)
		:
		index(index),
		transform(transform)
	{}
	unsigned int GetIndex() const
	{
		return index;
	}
	void SetTransform(Transform& transform_)
	{
		transform = transform_;
	}
	Transform& GetTransform()
	{
		return transform;
	}
private:
	unsigned int index;
	Transform transform;
};

class Joints
{
public:
	Joints() = default;
	Joints(std::vector<Joint> joints)
		:
		joints(joints)
	{}
	void AddJoint(Joint joint)
	{
		joints.push_back(joint);
	}
	void SetTransforms(std::vector<Transform> transforms)
	{
		for (unsigned int i = 0; i < joints.size();i++)
		{
			joints[i].SetTransform(transforms[i]);
		}
	}
	unsigned int Size() const
	{
		return joints.size();
	}
	
	unsigned int GetIndex(unsigned int i) const
	{
		return joints[i].GetIndex();
	}

	Transform GetTransform(unsigned int i)
	{
		return joints[i].GetTransform();
	}

	std::vector<Transform> GetTransforms()
	{
		std::vector<Transform> tfs;
		for (auto joint : joints)
			tfs.push_back(joint.GetTransform());
		return tfs;
	}
private:
	std::vector<Joint> joints;
};