// Morgan Ruffell - 2022, Bolt6.ai Test
// Base Pixel Shader - Sm5.1

struct VStoPS
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;
	float3 tangent		: TANGENT;
};

cbuffer externalData : register(b0)
{
	float3 RelativeCameraPosition;
	bool UseNormals;
	bool UseTangets;
}

bool DoRenderRoughness		= true;

Texture2D diffuseTexture	: register(t0);
Texture2D normalTexture		: register(t0);
Texture2D roughnessTexture	: register(t0);

SamplerState basicSampler	: register(s0);


float calculateSpecular(float3 normal, float3 worldPos, float3 dirToLight, float3 camPos)
{
	//ReflectionData reflectivityData;

	float3 dirToCamera = normalize(camPos - worldPos);
	float3 halfwayVector = normalize(dirToLight + dirToCamera);
	float shininess = 30.0f;

	return shininess == 0 ? 0.0f : pow(max(dot(halfwayVector, normal), 0), shininess);
}

float3 CalculateNormalFromMap(float2 uv, float3 normal, float3 tangent)
{
	float3 normalFromTexture = normalTexture.Sample(basicSampler, uv).xyz;
	float3 unpackedNormal = normalFromTexture * 2.0f - 1.0f;

	float3 FinalNormal = normal;
	float3 FinalTangent = normalize(tangent - FinalNormal * dot(tangent, FinalNormal));
	float3 FinalBitangent = cross(FinalNormal, FinalTangent);

	float3x3 TBN = float3x3(FinalTangent, FinalBitangent, FinalNormal);


	return normalize(mul(unpackedNormal, TBN));
}

float4 main(VStoPS input) : SV_TARGET
{
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	float3 finalNormal = CalculateNormalFromMap(input.uv, input.normal, input.tangent);

	input.normal = normalize(input.normal);
	finalNormal = normalize(finalNormal);

	if (DoRenderRoughness)
	{
		float roughness = roughnessTexture.Sample(basicSampler, input.uv).r;
	}

	//Break surface colour, and add the matrix with that of the final normal, before this time next week.

	return surfaceColor;
}