#ifndef GAMESCENE_H
#define GAMESCENE_H

class SceneGraph;
class GameScene
{
public:
	GameScene();
	SceneGraph& GetSceneGraph() { return *sceneGraph_; }

private:
	std::unique_ptr<SceneGraph> sceneGraph_;
};


#endif // GAMESCENE_H