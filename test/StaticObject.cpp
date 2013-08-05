#include "StaticObject.h"
#include "CompiledMesh.h"

StaticObject::StaticObject(std::unique_ptr<CompiledMesh> compiledMesh, core::matrix4x4 const& worldMatrix)
    : compiledMesh_(std::move(compiledMesh))
    , worldMatrix_(worldMatrix)
{

}

StaticObject::StaticObject(StaticObject&& other) 
    : compiledMesh_(std::move(other.compiledMesh_))
    , worldMatrix_(other.worldMatrix_)
{
}

core::matrix4x4 StaticObject::GetWorldMatrix() const
{
    return worldMatrix_;
}

CompiledMesh const& StaticObject::GetCompiledMesh() const
{
    return *compiledMesh_;
}