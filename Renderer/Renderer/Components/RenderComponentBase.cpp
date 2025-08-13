#include "pch.h"
#include "RenderComponentBase.h"

RenderComponentBase::RenderComponentBase(EComponentType componentType) 
	: Super(componentType)
{
}

RenderComponentBase::~RenderComponentBase()
{

}

void RenderComponentBase::BeginPlay()
{
	Super::BeginPlay();
}


void RenderComponentBase::Render()
{
}

void RenderComponentBase::RenderDepthOnly(bool bForPointLight, int32 instanceCount)
{
}

void RenderComponentBase::RenderDrawNormal()
{
}

void RenderComponentBase::Draw(UINT vertexCount, UINT startVertexLocation)
{
	CONTEXT->Draw(vertexCount, startVertexLocation);
}

void RenderComponentBase::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	CONTEXT->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void RenderComponentBase::DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	
}

void RenderComponentBase::DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	CONTEXT->DrawIndexedInstanced(indexCountPerInstance, indexCountPerInstance, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

