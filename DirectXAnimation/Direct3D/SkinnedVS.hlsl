cbuffer CBuf
{
	matrix model;
	matrix modelView;
	matrix modelViewProj;
	matrix pose[32];
	matrix invBindPose[32];
};


struct VSOut
{
	float3 worldPos : Position;
	float3 normal : Normal;
	float4 pos : SV_POSITION;
};
VSOut main(float3 pos : Position, float3 n : Normal, int4 jointID : Joint, float4 weight : Weight)
{
	VSOut vso;
	vso.worldPos = (float3)mul(float4(pos, 1.0f), modelView);
	vso.normal = mul(n, (float3x3)modelView);
	vso.pos = mul(float4(pos, 1.0f), modelViewProj);
	return vso;
}