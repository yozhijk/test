#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include <vector>
#include <string>
#include <memory>
#include <iosfwd>

#include "common_types.h"

#include "IGameEntity.h"

class Mesh;
class CompiledMesh;

class StaticObject : public IGameEntity
{
public:
	StaticObject(std::shared_ptr<CompiledMesh> compiledMesh, core::matrix4x4 const& worldMatrix);
	StaticObject(StaticObject&& other);

	core::matrix4x4 GetWorldMatrix() const;
	CompiledMesh const& GetCompiledMesh() const;

private:
	StaticObject(StaticObject const&);
	StaticObject& operator = (StaticObject const&);

	std::shared_ptr<CompiledMesh> compiledMesh_;
	core::matrix4x4 worldMatrix_;
};


#endif