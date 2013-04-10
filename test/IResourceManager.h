#ifndef IRESOURCEMANAGER_H
#define IRESOURCEMANAGER_H

#include <memory>

class CompiledModel;

class IResourceManager
{
public:
	virtual ~IResourceManager() = 0;

	/// Transform model into API-friendly form (would look into AssImp)
	virtual std::unique_ptr<CompiledModel> CompileModel(Model const& model) = 0;
};

inline IResourceManager::~IResourceManager(){}


#endif // RESOURCEMANAGER_H