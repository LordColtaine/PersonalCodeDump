#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
class Joint
{
public:
	Joint(int index, std::string name, DirectX::FXMMATRIX bindLocalTransform)
		:
		index(index),
		name(name),
		localBindTransform(bindLocalTransform)
	{}

	void AddChild(Joint joint) noexcept;
	DirectX::XMMATRIX GetAnimatedTransform() const noexcept;
	void SetAnimatedTransform(DirectX::FXMMATRIX animatedTransform) noexcept;
	DirectX::XMMATRIX GetInverseTransform() const noexcept;

protected:
	void CalculateInverseTransform(DirectX::FXMMATRIX ParentBindTransform);
private:
	int index;
	std::string name;
	std::vector<Joint> children;

	DirectX::XMMATRIX animatedTransform;
	DirectX::XMMATRIX localBindTransform;
	DirectX::XMMATRIX inverseBindTransform;
};