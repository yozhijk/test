#include "CompiledModel.h"

CompiledModel::CompiledModel(core::uint vertexBufferID, core::uint indexBufferID, ReleaseCallback releaseCallback) 
	: vertexBufferID_(vertexBufferID),
	  indexBufferID_(indexBufferID_),
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
	
inline core::uint CompiledModel::GetVertexBufferID() const
{
	return vertexBufferID_;
}

inline	core::uint CompiledModel::GetIndexBufferID() const
{
	return indexBufferID_;
}