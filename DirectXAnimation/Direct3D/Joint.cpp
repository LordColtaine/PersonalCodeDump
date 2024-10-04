#include "Joint.h"

void Joint::SetAnimatedTransform(DirectX::FXMMATRIX AnimatedTransform) noexcept
{
	animatedTransform = AnimatedTransform;
}

DirectX::XMMATRIX Joint::GetInverseTransform() const noexcept
{
	return inverseBindTransform;
}

void Joint::CalculateInverseTransform(DirectX::FXMMATRIX ParentBindTransform) noexcept
{
	DirectX::XMMATRIX bindTransform = DirectX::XMMatrixMultiply(ParentBindTransform, localBindTransform);
	inverseBindTransform = DirectX::XMMatrixTranspose(bindTransform);
	for (Joint child : children)
	{
		child.CalculateInverseTransform(bindTransform);
	}
}

DirectX::XMMATRIX Joint::GetAnimatedTransform() const noexcept
{
	return animatedTransform;
}

void Joint::AddChild(Joint joint)noexcept
{
	children.push_back(joint);
}

