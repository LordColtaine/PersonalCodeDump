#pragma once
#include "DrawableBase.h"
#include "ConstantBuffers.h"

class AssTest : public DrawableBase<AssTest>
{
public:
	AssTest(Graphics& gfx, DirectX::XMFLOAT3 material, float scale);
	void Update(const float t) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	float angle = 0;
	DirectX::XMFLOAT3X3 mt;
};