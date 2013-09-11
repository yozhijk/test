uniform mat4 g_mWorld;
uniform mat4 g_mWorldViewProj;

attribute vec3 inPosition;
attribute vec3 inNormal;
attribute vec2 inTexcoord;

varying vec3 Normal;
varying vec3 WorldPos;
varying vec2 Texcoord;

void main()
{
    gl_Position = vec4(inPosition, 1.0) * g_mWorldViewProj;
    WorldPos = (vec4(inPosition, 1.0) * g_mWorld).xyz;
    Normal = (vec4(inNormal, 0) * g_mWorld).xyz;
    Texcoord = inTexcoord;
}

