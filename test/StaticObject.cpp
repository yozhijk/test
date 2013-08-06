#include "StaticObject.h"
#include "CompiledMesh.h"

using namespace core;
using namespace std;

StaticObject::StaticObject(std::shared_ptr<CompiledMesh> compiledMesh, matrix4x4 const& worldMatrix)
    : compiledMesh_(compiledMesh)
    , worldMatrix_(worldMatrix)
{

}

StaticObject::StaticObject(StaticObject&& other) 
    : compiledMesh_(other.compiledMesh_)
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