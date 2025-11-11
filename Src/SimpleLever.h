/*
 *	@file	SimpleLever.h
 */

#ifndef _SIMPLELEVER_H_
#define	 _SIMPLELEVER_H_

#include "MapObject.h"
#include "Engine/Engine.h"

/*
 *	他のオブジェクトを操作するレバー
 */
class SimpleLever : public MapObject {
public:
	SimpleLever() = default;
	virtual ~SimpleLever() = default;

public:
	//	初期化
	virtual void Awake() override {
		GameObject* owner = GetOwner();
		Engine* engine = owner->GetEngine();

		owner->staticMesh = engine->GetStaticMesh(
			"Res/MeshData/lever/lever_arm.obj"
		);
		auto collider = owner->AddComponent<AABBCollider>();
		collider->aabb = {
			{ -0.5f, 0.0f, -0.5f },
			{  0.5f, 1.0f,  0.5f }
		};
		collider->isTrigger = true;

		auto cover = engine->Create<GameObject>(
			owner->name, owner->position, owner->rotation
		);
		cover->staticMesh = engine->GetStaticMesh(
			"Res/MeshData/lever/lever_cover.obj"
		);
	}
};

#endif // !_SIMPLELEVER_H_
