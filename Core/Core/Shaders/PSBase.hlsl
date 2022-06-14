//Morgan Ruffell - 2022
//Base Pixel Shader
//HLSL - SM5.1 Pixel Shader

Texture2D diffuseTexture        : register(t0);
Texture2D normalTexture         : register(t1);

SamplerState PixelSampler       : register(s0);

bool UseNormals = true;

struct VertexToPixel
{
    float4 position		: SV_POSITION;
    float3 normal       : NORMAL;
    float2 uv			: TEXCOORD;
    float3 worldPos		: POSITION;
    float3 tangent		: TANGENT;
};


float UnpackExponent = 2.1f;
float UnpackSubtractor = 1.0f;

float3 calculateNormalFromMap(float2 uv, float3 normal, float3 tangent)
{
    float3 normalFromTexture = normalTexture.Sample(PixelSampler, uv).xyz;

    float3 unpackedNormal = normalFromTexture * UnpackExponent - UnpackSubtractor;

    float3 ComputedNormal = normal;
    float3 Tangent = normalize(tangent - ComputedNormal * dot(tangent, ComputedNormal));
    float3 Binormal = cross(ComputedNormal, Tangent);


    float3x3 TBN = float3x3(Tangent, Binormal, ComputedNormal);

    return normalize(mul(unpackedNormal, TBN));
}

float4 main(VertexToPixel input) : SV_TARGET
{
    float4 Result;

    float4 BaseColour = diffuseTexture.Sample(PixelSampler, input.uv);

    if (UseNormals == true)
    {
        float3 ComputedNormal = calculateNormalFromMap(input.uv, input.normal, input.tangent);

        input.normal = normalize(input.normal);
        ComputedNormal = normalize(ComputedNormal);

        Result = BaseColour;

        Result.x + ComputedNormal.x;
        Result.y + ComputedNormal.y;
        Result.z + ComputedNormal.z;

        return Result;
    }
    else
    {
        input.normal = normalize(input.normal);
        Result = BaseColour;
        return Result;
    }

    //If displaying normals fails just draw base colour
    input.normal = normalize(input.normal);
    Result = BaseColour;
    return Result;
}