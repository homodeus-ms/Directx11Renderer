#include "Global.hlsli"
#include "Light.hlsli"

Texture2D preRendered : register(t30);
Texture2D depthMap : register(t31);

#define ORIGINAL_MODE 0
#define FOG_MODE 1
#define DEPTH_MODE 2

VertexUVOutput VS(uint id : SV_VertexID)
{
    float2 pos[6] = {
        float2(-1.0, -1.0),
        float2(-1.0, 1.0),
        float2(1.0, 1.0),
        float2(-1.0, -1.0),
        float2(1.0, 1.0),
        float2(1.0, -1.0)
    };

    float2 uv[6] = {
        float2(0.0, 1.0),
        float2(0.0, 0.0),
        float2(1.0, 0.0),
        float2(0.0, 1.0),
        float2(1.0, 0.0),
        float2(1.0, 1.0)
    };
    
    VertexUVOutput output;
    output.position = float4(pos[id], 0.f, 1.f);
    output.uv = uv[id];
    
    return output;
}

cbuffer PostEffectValues : register(CBUFFER_NUM_TEMPORAL)
{
    float3 fogColor;
    int mode;
    float depthScale;
    float fogStrength;
    float2 postEffectPad;
}

float4 UVToView(float2 uv)
{
    float4 posProj;

    // [0, 1]x[0, 1] -> [-1, 1]x[-1, 1]
    posProj.xy = uv * 2.0 - 1.0;
    posProj.y *= -1; // y 좌표 뒤집기
    posProj.z = depthMap.Sample(ClampSampler, uv).r;
    posProj.w = 1.0;
    
    float4 posView = mul(posProj, invP);
    posView.xyz /= posView.w;
    
    return posView;
}

float4 PS(VertexUVOutput input) : SV_Target
{
    if (mode == ORIGINAL_MODE)
    {
        float3 color = preRendered.Sample(ClampSampler, input.uv).rgb;
        return float4(color, 1.f);
    }
    else if (mode == FOG_MODE)
    {
        float4 posView = UVToView(input.uv);
        float dist = length(posView.xyz); // 눈의 위치가 원점인 좌표계
        
        // Fog
        float fogMin = 10.0;
        float fogMax = 30.0;
        
        float distFog = saturate((dist - fogMin) / (fogMax - fogMin));
        float fogFactor = exp(-distFog * fogStrength);

        float3 color = preRendered.Sample(ClampSampler, input.uv).rgb;
        color = lerp(fogColor, color, fogFactor);
        // color * fogFactor + fogColor * (1 - fogFactor)
        
        return float4(color, 1.0);
    }
    else // DEPTH_MODE
    {
        float z = UVToView(input.uv).z * depthScale;
        return float4(z, z, z, 1);
    }
}