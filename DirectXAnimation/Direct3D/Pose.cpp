#include "Pose.h"

Pose::Pose() { }

Pose::Pose(unsigned int numJoints) {
	Resize(numJoints);
}

Pose::Pose(const Pose& p) {
	*this = p;
}

Pose& Pose::operator=(const Pose& p) {
	if (&p == this) {
		return *this;
	}

	if (mParents.size() != p.mParents.size()) {
		mParents.resize(p.mParents.size());
	}
	if (mJoints.size() != p.mJoints.size()) {
		mJoints.resize(p.mJoints.size());
	}

	if (mJointIDs.size() != p.mJointIDs.size())
	{
		mJointIDs.resize(p.mJointIDs.size());
	}
	if (mParents.size() != 0) {
		memcpy(&mParents[0], &p.mParents[0],
			sizeof(int) * mParents.size());
	}
	if (mJoints.size() != 0) {
		memcpy(&mJoints[0], &p.mJoints[0],
			sizeof(Transform) * mJoints.size());
	}
	if (mJointIDs.size() != 0)
	{
		memcpy(&mJointIDs[0], &p.mJointIDs[0], sizeof(int) * mJointIDs.size());
	}

	return *this;
}

void Pose::Resize(unsigned int size) {
	mParents.resize(size);
	mJoints.resize(size);
	mJointIDs.resize(size);
	if (size)mParents[0] = -1;
}

unsigned int Pose::Size() {
	return mJoints.size();
}

Transform Pose::GetLocalTransform(unsigned int index) {
	return mJoints[index];
}

void Pose::SetLocalTransform(unsigned int index,
	const Transform& transform) {
	mJoints[index] = transform;
}

Transform Pose::GetGlobalTransform(unsigned int index) {
	Transform result = mJoints[index];
	for (int parent = mParents[index]; parent >= 0;
		parent = mParents[parent]) {
		result = combine(mJoints[parent], result);
	}

	return result;
}

Transform Pose::operator[](unsigned int index) {
	return GetGlobalTransform(index);
}

void Pose::GetMatrixPalette(std::vector<mat4>& out) {
	unsigned int size = Size();
	if (out.size() != size) {
		out.resize(size);
	}

	for (unsigned int i = 0; i < size; ++i) {
		Transform t = GetGlobalTransform(i);
		out[i] = transformToMat4(t);
	}
}

int Pose::GetParent(unsigned int index) {
	return mParents[index];
}

void Pose::SetParent(unsigned int index, int parent) {
	mParents[index] = parent;
}

int Pose::GetJointID(unsigned int index)
{
	return mJointIDs[index];
}

void Pose::SetJointID(unsigned int index, int ID)
{
	mJointIDs[index] = ID;
}

int Pose::GetIndexFromJointID(const int ID)
{
	int index = 0;
	for (; index < mJointIDs.size(); index++)
	{
		if (ID == mJointIDs[index])
			break;
	}
	if (mJointIDs.size() == index) index = -1;
	return index;
}
bool Pose::operator==(const Pose& other) {
	if (mJoints.size() != other.mJoints.size()) {
		return false;
	}
	if (mParents.size() != other.mParents.size()) {
		return false;
	}
	unsigned int size = (unsigned int)mJoints.size();
	for (unsigned int i = 0; i < size; ++i) {
		Transform thisLocal = mJoints[i];
		Transform otherLocal = other.mJoints[i];

		int thisParent = mParents[i];
		int otherParent = other.mParents[i];

		if (thisParent != otherParent) {
			return false;
		}

		if (thisLocal != otherLocal) {
			return false;
		}
	}
	return true;
}

bool Pose::operator!=(const Pose& other) {
	return !(*this == other);
}
