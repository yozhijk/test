#ifndef IRESOURCEMANAGER_H
#define IRESOURCEMANAGER_H

#include <memory>

class Mesh;
class CompiledMesh;

class IResourceManager
{
public:
    virtual ~IResourceManager() = 0;

    /// Transform model into API-friendly form (would look into AssImp)
    virtual std::unique_ptr<CompiledMesh> CompileMesh(Mesh const& mesh) = 0;
};

inline IResourceManager::~IResourceManager(){}


#endif // RESOURCEMANAGER_H