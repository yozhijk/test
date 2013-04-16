#ifndef SHADERCOMMON_H
#define SHADERCOMMON_H

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


cbuffer PerFrameCB : register(c0)
{
    float4x4 g_mWorld;
    float4x4 g_mWorldViewProj;
};

#endif