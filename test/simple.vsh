#include "ShaderCommon.h"

PSInput main(VSInput input)
{
	PSInput output;
	output.vPos = mul(float4(input.vPos, 1), g_mWorldViewProj);
	output.vNorm = mul(float4(input.vNorm, 0), g_mWorld).xyz;
	output.vWorldPos =  mul(float4(input.vPos, 1), g_mWorld).xyz;
	output.vTex = input.vTex;
	return output;
}
