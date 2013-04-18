#include "ShaderCommon.h"

PSInput main(VSInput input)
{
    PSInput output;
    output.pos = mul(float4(input.pos, 1), g_mWorldViewProj);
    output.normal = mul(float4(input.normal, 0), g_mWorld).xyz;
    output.tex = input.tex;
    return output;
}
