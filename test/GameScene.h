#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "common_types.h"
#include <vector>
//#include "SceneGraph.h"

class CompiledMesh;
class IGraphicsContext;

class GameScene
{
public:
	GameScene();

	void Render(IGraphicsContext& graphicsContext);
	void Update(core::real timeDelta);

private:
	// SceneGraph sceneGraph_;
	// static geometry
	// dynamic objects
	// etc
	std::vector<CompiledMesh> staticObjects_;
};


#endif // GAMESCENE_H