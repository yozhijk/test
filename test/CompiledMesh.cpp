#include "CompiledMesh.h"

using namespace core;

CompiledMesh::CompiledMesh(uint vertexBufferID, uint indexBufferID, uint indexCount, uint vertexSizeInBytes, ReleaseCallback releaseCallback)
	: vertexBufferID_(vertexBufferID)
	,indexBufferID_(indexBufferID)
	,indexCount_(indexCount)
	,vertexSizeInBytes_(vertexSizeInBytes)
	,releaseCallback_(releaseCallback)
{
}

CompiledMesh::~CompiledMesh()
{
	if (releaseCallback_)
	{
		releaseCallback_(*this);
	}
}

uint CompiledMesh::GetVertexBufferID() const
{
	return vertexBufferID_;
}

uint CompiledMesh::GetIndexBufferID() const
{
	return indexBufferID_;
}

uint CompiledMesh::GetIndexCount() const
{
	return indexCount_;
}

uint CompiledMesh::GetVertexSizeInBytes() const
{
	return vertexSizeInBytes_;
}