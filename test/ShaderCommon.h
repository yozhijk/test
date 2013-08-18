#ifndef SHADERCOMMON_H
#define SHADERCOMMON_H

#define POINT_LIGHTS_MAX 4

struct VSInput
{
	float3 vPos : POSITION;
	float2 vTex : TEXCOORD;
	float3 vNorm : NORMAL;
};

struct PSInput
{
	float4 vPos : SV_Position;
	float2 vTex : TEXCOORD;
	float3 vNorm : NORMAL;
	float3 vWorldPos : WORLDPOS;
};

struct PointLight
{
	float4 vPos;
	float4 vColor;
};

cbuffer PerFrameCB : register(b0)
{
	float4x4 g_mWorld;
	float4x4 g_mWorldViewProj;
};

cbuffer LightCB : register(b1)
{
	PointLight g_PointLights[POINT_LIGHTS_MAX];
};



#endif