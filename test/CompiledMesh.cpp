#include "CompiledMesh.h"

CompiledMesh::CompiledMesh(core::uint vertexBufferID, core::uint indexBufferID, ReleaseCallback releaseCallback) 
	: vertexBufferID_(vertexBufferID),
	  indexBufferID_(indexBufferID),
	   releaseCallback_(releaseCallback)
{

}
	
CompiledMesh::~CompiledMesh()
{
	if (releaseCallback_)
	{
		releaseCallback_(*this);
	}
}
	
core::uint CompiledMesh::GetVertexBufferID() const
{
	return vertexBufferID_;
}

core::uint CompiledMesh::GetIndexBufferID() const
{
	return indexBufferID_;
}