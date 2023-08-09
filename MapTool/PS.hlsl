Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

float4 main(VSOutput vsOutput) : SV_TARGET
{
	float4 result = baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
	result = float4(result.x * vsOutput.color.w, result.y * vsOutput.color.w, result.z * vsOutput.color.w, 1.f);


	return result;
}
