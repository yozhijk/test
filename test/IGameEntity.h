#ifndef GAMEENTITY_H
#define GAMEENTITY_H

class IGameEntity
{
public:
	virtual ~IGameEntity() = 0;
};

inline IGameEntity::~IGameEntity() {} 


#endif // GAMEENTITY_H