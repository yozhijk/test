#ifndef COMPILEDMODEL_H
#define COMPILEDMODEL_H

#include "common_types.h"
#include <functional>

class CompiledModel
{
public:
	typedef std::function<void (CompiledModel const&)> ReleaseCallback;

	CompiledModel(core::uint vertexBufferID, core::uint indexBufferID, ReleaseCallback releaseCallback = nullptr);
	~CompiledModel();

	core::uint GetVertexBufferID() const;
	core::uint GetIndexBufferID() const;

private:
	CompiledModel(CompiledModel const&);
	CompiledModel& operator = (CompiledModel const&);

	core::uint vertexBufferID_;
	core::uint indexBufferID_;
	ReleaseCallback releaseCallback_;
};



#endif // COMPILEDMODEL_H