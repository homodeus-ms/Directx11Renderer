#include "FilterResources.hlsli"

VertexUVOutput VS(VertexUVInput input)
{
    VertexUVOutput output;
    output.position = input.position;
    output.uv = input.uv;

    return output;
}