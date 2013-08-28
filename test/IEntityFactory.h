#ifndef IENTITYFACTORY_H
#define IENTITYFACTORY_H

#include <memory>

class ICamera;

template <typename Stream> class IEntityFactory
{
public:
	virtual ~IEntityFactory() = 0;
	virtual std::unique_ptr<ICamera> CreateCamera(Stream& stream) = 0;

private:
	IEntityFactory(IEntityFactory const&);
	IEntityFactory& operator=(IEntityFactory const&);
};

template <typename Stream> inline IEntityFactory<Stream>::~IEntityFactory(){};

#endif //IENTITYFACTORY_H