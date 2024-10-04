
cbuffer CBuf
{
	float4 face_colors[6];
};

float4 main(uint tid : SV_PRIMITIVEID) : SV_TARGET
{
	return float4(0.5f, 0.5f, 0.5f, 1.0f);
}