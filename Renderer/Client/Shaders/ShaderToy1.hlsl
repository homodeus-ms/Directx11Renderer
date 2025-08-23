cbuffer ShaderToyUniforms : register(b10)
{
    float3 iResolution; // (width, height, depth)
    float iTime;
    float iTimeDelta; // render time (in seconds)
    float iFrameRate; // shader frame rate
    int iFrame;
    float _padding;
};

struct VSOut
{
    float4 position : SV_POSITION; // 화면 좌표
    float2 uv : TEXCOORD0; // 필요시
};

//----------------------------------------------
// GLSL → HLSL 치환 헬퍼
//----------------------------------------------
#define saturate(x) clamp(x, 0.0, 1.0)
#define pack(x) ((x) * 0.5 + 0.5)
#define unpack(x) ((x) * 2.0 - 1.0)
#define lerp3(a,b,x) lerp(a,b,x)   // vec3 전용
#define rgb(r,g,b) (float3(r,g,b) * 0.0039215686)

// fract → frac
float fract(float x)
{
    return frac(x);
}
float2 fract(float2 x)
{
    return frac(x);
}
float3 fract(float3 x)
{
    return frac(x);
}
float4 fract(float4 x)
{
    return frac(x);
}

// dot, sin, cos, floor, etc. 그대로 사용 가능

//----------------------------------------------
// 공용 함수 (GLSL 그대로 이식)
//----------------------------------------------
float smootherstep(float a, float b, float x)
{
    x = saturate((x - a) / (b - a));
    return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
}

float segment(float value, float segments)
{
    return (float) (int(value * segments)) / segments;
}

float3 sdgCircle(float2 p, float r)
{
    float d = length(p);
    return float3(d - r, p / d);
}

float sdCircle(float2 p, float r)
{
    return length(p) - r;
}

float sdRoundedBox(float2 p, float2 b, float4 r)
{
    r.xy = (p.x > 0.0) ? r.xy : r.zw;
    r.x = (p.y > 0.0) ? r.x : r.y;
    float2 q = abs(p) - b + r.x;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r.x;
}

float pcurve(float x, float a, float b)
{
    float k = pow(a + b, a + b) / (pow(a, a) * pow(b, b));
    return k * pow(x, a) * pow(1.0 - x, b);
}

float rand2(float2 p)
{
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

float2 hash21(float p)
{
    float3 p3 = frac(float3(p, p, p) * float3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return frac((p3.xx + p3.yz) * p3.zy);
}

float2 hash2(float2 p)
{
    return frac(sin(float2(
        dot(p, float2(127.1, 311.7)),
        dot(p, float2(269.5, 183.3))
    )) * 43758.5453);
}

// simplex noise optimized
float3 permute_optimizedSnoise2D(float3 x)
{
    return fmod((x * x * 34.0 + x), 289.0);
}

float optimizedSnoise(float2 v)
{
    float2 i = floor((v.x + v.y) * .36602540378443 + v);
    float2 x0 = (i.x + i.y) * .211324865405187 + v - i;
    float s = step(x0.x, x0.y);
    float2 j = float2(1.0 - s, s);
    float2 x1 = x0 - j + .211324865405187;
    float2 x3 = x0 - .577350269189626;
    i = fmod(i, 289.0);
    float3 p = permute_optimizedSnoise2D(
                   permute_optimizedSnoise2D(i.y + float3(0, j.y, 1))
                 + i.x + float3(0, j.x, 1));
    float3 m = max(0.5 - float3(dot(x0, x0), dot(x1, x1), dot(x3, x3)), 0.0);
    float3 x = frac(p * .024390243902439) * 2.0 - 1.0;
    float3 h = abs(x) - 0.5;
    float3 a0 = x - floor(x + 0.5);
    return 0.5 + 65.0 * dot(pow(m, float3(4.0, 4.0, 4.0)) *
           (-0.8537347 * (a0 * a0 + h * h) + 1.7928429),
           a0 * float3(x0.x, x1.x, x3.x) + h * float3(x0.y, x1.y, x3.y));
}

float getH(float pos, out float2 from, out float2 to, out float blend)
{
    float n;
    float i = floor(pos);
    float f = pos - i;
    float2 rand = float2(0.4, 0.95);

    float2 sub = float2(0.5, 0.0);
    float2 add = float2(0.5, 1.1 - rand.y);
    float2 l = (hash21(i - 1.0) - sub) * rand + add;
    float2 c = (hash21(i) - sub) * rand + add;
    float2 r = (hash21(i + 1.0) - sub) * rand + add;

    l.x = (i - 1.0) + l.x;
    c.x = i + c.x;
    r.x = (i + 1.0) + r.x;

    if (pos < c.x)
    {
        from = l;
        to = c;
    }
    else
    {
        from = c;
        to = r;
    }

    float tl = 0.5 * (to.x - from.x - to.y + from.y);
    float2 mid = to + float2(-1.0, 1.0) * tl;

    if (pos < mid.x)
    {
        to = mid;
    }
    else
    {
        from = mid;
    }

    blend = ((pos - from.x) / (to.x - from.x));
    n = lerp(from.y, to.y, blend);

    return n;
}

//----------------------------------------------
// ShaderToy mainImage()
//----------------------------------------------
//void mainImage(out float4 fragColor, in float2 fragCoord)

float4 PS(VSOut input) : SV_Target
{
    input.uv.y = 1.f - input.uv.y;
    float2 fragCoord = input.uv * iResolution.xy;
    float mx = max(iResolution.x, iResolution.y);
    float2 uv = fragCoord / mx;
    float2 nuv = fragCoord / iResolution.xy;
    float2 pos = uv - (iResolution.xy) * 0.5 / mx;
    
    float4 fragColor = { 0.f, 0.f, 0.f, 1.f };
    float col = 1.0;

    float sNoise = optimizedSnoise(float2(pos.x * 15.0, 0.0));

    // Sun
    float3 circle = sdgCircle(pos + float2(0.2, -0.05), 0.1);
    col *= saturate(400.0*abs(circle.x) - 1.0*(0.5 + sin(sNoise*5.0 + 0.5*iTime)));
    col *= saturate(1.3 - saturate(-circle.x*800.0 - 7.0)*cos(circle.x*800.0 - 1.0*(1.0 + cos(sNoise*5.0+0.5*iTime))));

    // deform
    pos -= 0.5 * float2(pos.y, -pos.x);
    pos.x += iTime * 0.01;

    float scaleX = 5.0;
    float2 from, to;
    float blend;
    float noise = getH(scaleX * pos.x, from, to, blend);
    noise -= 0.05 * sNoise;

    float posY = 0.3;
    float scaleY = 1.0 / scaleX;
    float scaledNoise = scaleY * noise;
    float mountHeight = posY + scaledNoise;

    float hatchLength = length(to - from);
    float hatchWidth = lerp(0.5, 3.0, 1.0 - saturate(pcurve(blend, 2.0*hatchLength, 2.0)));
    col = min(col, iResolution.y * (abs(nuv.y - mountHeight) - hatchWidth / iResolution.y));

    float mountGrad = saturate(scaledNoise - nuv.y + posY) / scaledNoise;
    if ((nuv.y < mountHeight - 0.0025) && (nuv.y > posY))
    {
        col = ((lerp(0.5, 3.0, mountGrad) * abs(cos((1.0 - pow(0.025 * sNoise + mountGrad, 0.5)) * (lerp(1.0, noise, 0.8)) * 3.14159 * 25.0))
              + saturate(3.0 + 10.0*cos(pos.x * 32.0 - cos(pow(mountGrad, 1.25)*13.0)))
              - saturate(3.0 + 10.0*cos(2.0 + pos.x * 32.0 - cos(pow(mountGrad, 1.25)*13.0)))
        ));
    }

    // Horizon
    float belowHorizon = saturate(1.0 - (posY - nuv.y) / posY + 0.01*sNoise);
    float aboveHorizonMask = saturate(iResolution.y*(belowHorizon - 1.0 + 1.0/iResolution.y));
    col = min(col, max(aboveHorizonMask, saturate(max(
        saturate(1.1 - belowHorizon*belowHorizon),
        saturate(abs(cos(0.2*belowHorizon*belowHorizon*3.14159*450.0*posY)))
        + saturate(0.5 + cos(pos.x * 27.0 + cos(pow(belowHorizon,3.0)*13.0)))
        - saturate(-0.5 + cos(pos.x * 32.0 + cos(pow(belowHorizon,3.0)*13.0)))
    ))));

    // Clouds
    float clouds = saturate(
        2.0*abs(optimizedSnoise(nuv * float2(2.0, 4.0) + float2(0.05*iTime, 0.0)) - 0.5)
      + 1.0*optimizedSnoise(nuv * float2(5.0, 16.0) + float2(0.2*iTime, 0.0))
    );
    col = max(col, saturate((nuv.y + 0.25)*clouds*clouds - 1.0 + clouds));

    fragColor.a = 1.0;
    fragColor.rgb = lerp(float3(0.0, 0.2, 0.45),
                         float3(0.8, 0.85, 0.89),
                         1.0 - col);

    fragColor.rgb += 1.5 * 0.75 * ((rand2(uv) - .5) * .07);

    float2 vigenteSize = 0.3 * iResolution.xy;
    float sdf = -sdRoundedBox(fragCoord - iResolution.xy * 0.5, vigenteSize,
                              float4(0.25 * min(iResolution.x, iResolution.y),
                                     0.25 * min(iResolution.x, iResolution.y),
                                     0.25 * min(iResolution.x, iResolution.y),
                                     0.25 * min(iResolution.x, iResolution.y))) / vigenteSize.x;
    float percent = 0.8;
    sdf = (saturate(percent + sdf) - percent) / (1.0 - percent);
    sdf = lerp(1.0, sdf, 0.05);
    fragColor.rgb *= sdf;
    
    return float4(fragColor.rgb, 1.f);

}
