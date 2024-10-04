#pragma once
#include "DrawableBase.h"


class SolidSphere :public DrawableBase<SolidSphere>
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void Update(float dt) noexcept override;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	DirectX::XMFLOAT3 GetPos()
	{
		return pos;
	}
private:
	DirectX::XMFLOAT3 pos = { 1.0f, 1.0f, 1.0f };
};