#include "CompiledModel.h"

CompiledModel::CompiledModel(core::uint vertexBufferID, core::uint indexBufferID, ReleaseCallback releaseCallback) 
	: vertexBufferID_(vertexBufferID),
	  indexBufferID_(indexBufferID),
	   releaseCallback_(releaseCallback)
{

}
	
CompiledModel::~CompiledModel()
{
	if (releaseCallback_)
	{
		releaseCallback_(*this);
	}
}
	
core::uint CompiledModel::GetVertexBufferID() const
{
	return vertexBufferID_;
}

core::uint CompiledModel::GetIndexBufferID() const
{
	return indexBufferID_;
}