#ifndef SHADERCOMMON_H
#define SHADERCOMMON_H

#define POINT_LIGHTS_MAX 4
#define SPOT_LIGHTS_MAX 4

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

struct SpotLight
{
	float4 vPos;
	float4 vDir;
	float4 vColor;
	float4 vAngle;
};

cbuffer PerFrameCB : register(b0)
{
	float4x4 g_mWorld;
	float4x4 g_mWorldViewProj;
};

cbuffer PointLightCB : register(b1)
{
	PointLight g_PointLights[POINT_LIGHTS_MAX];
};

cbuffer SpotLightCB : register(b2)
{
	SpotLight g_SpotLights[SPOT_LIGHTS_MAX];
};



#endif