#include "CompiledMesh.h"

CompiledMesh::CompiledMesh(core::uint vertexBufferID, core::uint indexBufferID, core::uint indexCount, core::uint vertexSizeInBytes, ReleaseCallback releaseCallback)
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

core::uint CompiledMesh::GetVertexBufferID() const
{
    return vertexBufferID_;
}

core::uint CompiledMesh::GetIndexBufferID() const
{
    return indexBufferID_;
}

core::uint CompiledMesh::GetIndexCount() const
{
    return indexCount_;
}

core::uint CompiledMesh::GetVertexSizeInBytes() const
{
    return vertexSizeInBytes_;
}