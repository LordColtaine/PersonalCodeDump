#pragma once
#include "GraphicsThrowMacros.h"
#include "Cube.h"
#include "BindableBase.h"
#include "OBJLoader.h"
#include "Vertex.h"
using namespace Bind;
class DrawCube : public DrawableBase<DrawCube>
{
public: DirectX::XMFLOAT3 material;
public:
	DrawCube(Graphics& gfx, DirectX::XMFLOAT3 material)
		:
		material(material)
	{
		namespace dx = DirectX;

		if (!IsStaticInitialized())
		{
			struct Vertex
			{
				dx::XMFLOAT3 pos;
				dx::XMFLOAT3 n;
			};
			//Object model;
			//model.LoadFromFile("C:\\Users\\K Raviraju Prasanna\\Documents\\OBJFiles\\al.obj");
			auto model = Cube::MakeIndependent<Vertex>();
			model.SetNormalsIndependentFlat();
			AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));
			
			auto pvs = std::make_unique<VertexShader>(gfx, L"LightVS.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<PixelShader>(gfx, L"LightPS.cso"));

			
			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
			
			AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticBind(make_unique<Sampler>(gfx));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(std::make_unique<TransformCbuf>(gfx, *this));

		struct PSMaterialConstant
		{
			alignas(16) dx::XMFLOAT3 color;
			float specularIntensity = 1.f;
			float specularPower = 30.0f;
			float padding[2];
		} colorConst;
		colorConst.color = material;
		AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
		dx::XMStoreFloat3x3(
			&mt,
			dx::XMMatrixScaling(1.0f, 1.0f, 1.0f)
		);
	}

	void Update(const float t) noexcept override
	{
		angle += t;
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept override
	{
		namespace dx = DirectX;
		return dx::XMLoadFloat3x3(&mt) *
			dx::XMMatrixRotationY(angle) *
			dx::XMMatrixTranslation(0.0f, 0.0f, 3.0f) *
		dx::XMMatrixRotationX(angle);
	}
private:
	float angle = 0;
	DirectX::XMFLOAT3X3 mt;
};