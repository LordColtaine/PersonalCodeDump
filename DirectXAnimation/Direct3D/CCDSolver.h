#pragma once
#include "math\Transform.h"
#include <vector>

class CCDSolver {
protected:
	
	unsigned int mNumSteps;
	float mThreshold;
public:
	std::vector<Transform> mIKChain;
	CCDSolver();
	unsigned int Size();
	void Resize(unsigned int newSize);

	Transform& operator[](unsigned int index);
	Transform GetGlobalTransform(unsigned int index);

	unsigned int GetNumSteps();
	void SetNumSteps(unsigned int numSteps);

	float GetThreshold();
	void SetThreshold(float value);
	bool Solve(const Transform& target);
};

