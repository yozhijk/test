#ifndef COMPILEDMESH_H
#define COMPILEDMESH_H

#include "common_types.h"
#include <functional>

class CompiledMesh
{
public:
    typedef std::function<void (CompiledMesh const&)> ReleaseCallback;

    CompiledMesh(core::uint vertexBufferID, core::uint indexBufferID, core::uint indexCount, core::uint vertexSizeInBytes, ReleaseCallback releaseCallback = nullptr);
    ~CompiledMesh();

    core::uint GetVertexBufferID() const;
    core::uint GetIndexBufferID() const;
    core::uint GetIndexCount() const;
    core::uint GetVertexSizeInBytes() const;

private:
    CompiledMesh(CompiledMesh const&);
    CompiledMesh& operator = (CompiledMesh const&);

    core::uint vertexBufferID_;
    core::uint indexBufferID_;
    core::uint indexCount_;
    core::uint vertexSizeInBytes_;
    ReleaseCallback releaseCallback_;
};



#endif // COMPILEDMODEL_H