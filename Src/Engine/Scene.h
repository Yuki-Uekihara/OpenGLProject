/*
 *	@file	Scene.h
 */

#ifndef _SCENE_H_
#define _SCENE_H_

#include <memory>

/*
 *	シーンの基底クラス
 */
class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;

public:
	virtual bool Initialize(class Engine& engine) { return true; }
	virtual void Update(class Engine& engine, float deltaTime) {}
	virtual void Finalize(class Engine& engine) {}
};
//	別名定義
using ScenePtr = std::shared_ptr<Scene>;


#endif // !_SCENE_H_
