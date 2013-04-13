#ifndef COMPILEDMESH_H
#define COMPILEDMESH_H

#include "common_types.h"
#include <functional>

class CompiledMesh
{
public:
	typedef std::function<void (CompiledMesh const&)> ReleaseCallback;

	CompiledMesh(core::uint vertexBufferID, core::uint indexBufferID, core::uint vertexCount, ReleaseCallback releaseCallback = nullptr);
	~CompiledMesh();

	core::uint GetVertexBufferID() const;
	core::uint GetIndexBufferID() const;
    core::uint GetVertexCount() const;

private:
	CompiledMesh(CompiledMesh const&);
	CompiledMesh& operator = (CompiledMesh const&);

	core::uint vertexBufferID_;
	core::uint indexBufferID_;
    core::uint vertexCount_;
	ReleaseCallback releaseCallback_;
};



#endif // COMPILEDMODEL_H