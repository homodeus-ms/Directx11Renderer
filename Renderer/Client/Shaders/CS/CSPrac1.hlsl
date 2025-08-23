RWTexture2D<float4> gOutput : register(u0);

cbuffer scaleBuffer : register(b0)
{
    float scale;
}

[numthreads(32, 32, 1)]
void main(uint3 gID : SV_GroupID, uint3 dtID : SV_DispatchThreadID )
{
    
    if (gID.x % 2 == 0)
    {
        if (gID.y % 2 == 0)
            gOutput[dtID.xy] = float4(.5f, .5f, .5f, 1.f);
        else
            gOutput[dtID.xy] = float4(0.f, 0.f, 0.f, 1.f);
    }
    else
    {
        if (gID.y % 2 == 0)
            gOutput[dtID.xy] = float4(0.f, 0.f, 0.f, 1.f);
        else
            gOutput[dtID.xy] = float4(.5f, .5f, .5f, 1.f);
    }
}