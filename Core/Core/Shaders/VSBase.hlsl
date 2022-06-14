//Morgan Ruffell - 2022
//Base Vertex Shader - Animation
//HLSL - SM5.1 Vertex Shader

int ShaderMaximumOfActiveBones = 30;

struct VertexShaderInput
{
	float4 position		: POSITION;
	float3 normal       : NORMAL;

	float4 boneid		: BONEID;
	float4 weight		: WEIGHT;

	float2 uv			: TEXCOORD;
	float2 uv1			: TEXCOORD1;

	float3 tangent		: TANGENT;
};

//Following the render pipeline, at it's most primitive
//This goes to the pixel shader, you can have tessalation options
//but we're going for simplicity.
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal       : NORMAL;

	float2 uv			: TEXCOORD;
	float2 uv1			: TEXCOORD1;

	float3 worldPos		: POSITION;
	float3 tangent		: TANGENT;
};

//This must match the inside of the C++ struct or it will not compile
//as we are sending that data to the GPU in here.
struct Bone
{
	matrix BoneTransform;
	matrix InvBoneTransform;
	int NumberOfChildren;
	int MaximumNumberOfChildBones;
};

cbuffer WorldData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

//This is that same data mapped to a GPU registry -- These are 16 bytes aligned
cbuffer bones : register(b1)
{
	Bone bones[40];
	float blendWeight; //The blending of transformation per bone
}

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	matrix worldViewProj = mul(mul(world, view), projection);
	matrix bonetransform = 0;

	if (!bones[0].NumberOfChildren > ShaderMaximumOfActiveBones)
	{
		//These scales the vertex bound to a bone through a blendshape by the position in the bonetransform matrix.
		if (input.boneid.x != -1)
		{
			bonetransform = mul(mul(bones[input.boneid.x].BoneTransform, input.weight.x), bones[input.boneid.x].InvBoneTransform) * blendWeight;
		}

		if (input.boneid.y != -1)
		{
			bonetransform += mul(mul(bones[input.boneid.y].BoneTransform, input.weight.y), bones[input.boneid.y].InvBoneTransform) * blendWeight;
		}

		if (input.boneid.z != -1)
		{
			bonetransform += mul(mul(bones[input.boneid.z].BoneTransform, input.weight.z), bones[input.boneid.z].InvBoneTransform) * blendWeight;
		}

		output.position = mul(mul(bonetransform, input.position), worldViewProj); //We have to multiply the vertexes position in 3D space by the world view projection matrix
		output.normal = mul((float3)mul(bonetransform, float4(input.normal, 1.0)), (float3x3)world);
		output.worldPos = mul(input.position, world).xyz;

		output.uv = input.uv;
		output.uv1 = input.uv1;

		output.tangent = normalize(mul((float3)mul(bonetransform, float4(input.tangent, 2.0)), (float3x3)world));

		return output;
	}

	//If there are too many bones for the shader to handle, which I've limited because of the GPU requirements, then just apply minimal operations per vertex
	output.position = input.position; //We have to multiply the vertexes position in 3D space by the world view projection matrix
	output.normal = input.normal;
	output.worldPos = mul(input.position, world).xyz;

	output.uv = input.uv;
	output.uv1 = input.uv1;

	output.tangent = input.tangent;

	return output;
}

