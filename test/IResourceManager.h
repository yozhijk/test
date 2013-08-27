#ifndef IRESOURCEMANAGER_H
#define IRESOURCEMANAGER_H

#include <memory>

class Mesh;
class CompiledMesh;
class Texture2D;

class IResourceManager
{
public:
	virtual ~IResourceManager() = 0;

	/// Transform model into API-friendly form (would look into AssImp)
	virtual std::shared_ptr<CompiledMesh> CompileMesh(Mesh const& mesh) = 0;
	//virtual std::shared_ptr<Texture2D> CreateTexture2D(core::uint width, core::uint height, core::uint numChannels, void* initData) = 0;
};

inline IResourceManager::~IResourceManager(){}


#endif // RESOURCEMANAGER_H