#ifndef SHADERCOMMON_H
#define SHADERCOMMON_H

#define POINT_LIGHTS_MAX 4

struct VSInput
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
};

struct PSInput
{
    float4 pos : SV_Position;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
};

struct PointLight
{
	float4 vPos;
	float4 vColor;
};

cbuffer PerFrameCB : register(c0)
{
    float4x4 g_mWorld;
    float4x4 g_mWorldViewProj;
};

cbuffer LightCB : register(c1)
{
	PointLight g_PointLights[POINT_LIGHTS_MAX];
};



#endif