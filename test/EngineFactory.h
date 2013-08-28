#ifndef ENGINEFACTORY_H
#define ENGINEFACTORY_H

#include "IEntityFactory.h"

template <typename Stream> class EngineFactory : public IEntityFactory<Stream>
{
public:
	EngineFactory();
	~EngineFactory();

	virtual std::unique_ptr<ICamera> CreateCamera(Stream& stream);

private:
	EngineFactory(EngineFactory const&);
	EngineFactory& operator = (EngineFactory const&);
};

#endif // ENGINEFACTORY_H