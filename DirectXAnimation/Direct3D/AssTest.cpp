#include "AssTest.h"
#include "BindableBase.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

using namespace Bind;
AssTest::AssTest(Graphics& gfx, DirectX::XMFLOAT3 material, float scale)
{
	namespace dx = DirectX;
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};
		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("C:\\Users\\K Raviraju Prasanna\\Documents\\OBJFiles\\BrainStem.gltf",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		/*const auto pModel = imp.ReadFile("C:\\Users\\K Raviraju Prasanna\\Documents\\OBJFiles\\al.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);*/
		const auto pMesh = pModel->mMeshes[0];

		std::vector<Vertex> vertices;
		vertices.reserve(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back(
				{
					{pMesh->mVertices[i].x * scale, pMesh->mVertices[i].y * scale, pMesh->mVertices[i].z * scale},
					*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[i])
				}
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(3 == face.mNumIndices);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

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
		AddStaticBind(std::make_unique<Sampler>(gfx));

		struct PSMaterialConstant
		{
			alignas(16) dx::XMFLOAT3 color;
			float specularIntensity = 1.f;
			float specularPower = 30.0f;
			float padding[2];
		} colorConst;
		colorConst.color = material;
		AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
	}
	else
	{
		SetIndexFromStatic();
	}
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(10.0f, 10.0f, 10.0f)
	);
}

void AssTest::Update(const float t) noexcept
{
	angle += t;
}

DirectX::XMMATRIX AssTest::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3(&mt) *
		dx::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		//dx::XMMatrixRotationX(angle);
}