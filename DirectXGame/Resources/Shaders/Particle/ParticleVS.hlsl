#include "Particle.hlsli"

VSOutput main(float3 pos : POSITION, float scale : SCALE, float4 color : COLOR)
{
    VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.pos = float4(pos, 1);
    output.scale = scale;
    output.color = color;
    return output;
}