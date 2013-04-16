#include "CompiledMesh.h"

CompiledMesh::CompiledMesh(core::uint vertexBufferID, core::uint indexBufferID, core::uint vertexCount, ReleaseCallback releaseCallback)
    : vertexBufferID_(vertexBufferID)
    ,indexBufferID_(indexBufferID)
    ,vertexCount_(vertexCount)
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

core::uint CompiledMesh::GetVertexBufferID() const
{
    return vertexBufferID_;
}

core::uint CompiledMesh::GetIndexBufferID() const
{
    return indexBufferID_;
}

core::uint CompiledMesh::GetVertexCount() const
{
    return vertexCount_;
}