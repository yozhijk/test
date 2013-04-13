uniform mat4 g_mWorld;
uniform mat4 g_mWorldViewProj;

attribute vec3 inPosition;
attribute vec3 inNormal;
attribute vec2 inTexcoord;

varying vec3 Normal;
varying vec2 Texcoord;

void main()
{
    gl_Position = g_mWorldViewProj * vec4(inPosition, 1.0);
    Normal = (g_mWorld * vec4(inNormal, 0)).xyz;
    Texcoord = inTexcoord;
}

