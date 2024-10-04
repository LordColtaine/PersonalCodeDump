#pragma once
#include "Joint.h"

namespace dx = DirectX;
struct Vertex
{
	dx::XMFLOAT3 position;
	dx::XMFLOAT2 texCoords;
	dx::XMFLOAT3 normals;
	dx::XMINT3 joints;
	dx::XMFLOAT3 weights;
};
class AnimatedModel
{


private:
	Vertex model;

};