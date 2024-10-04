
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};

cbuffer ConstantBuffer
{
	matrix transform;
};

VS_OUTPUT main( float3 pos : POSITION)
{
	VS_OUTPUT vso;
	vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), transform);
	return vso;
}