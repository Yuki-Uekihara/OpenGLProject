/*
 *	@file	RemoteDoor.h
 */

#ifndef _REMOTEDOOR_H_
#define	_REMOTEDOOR_H_

#include "MapObject.h"
#include "Engine/Engine.h"

 /*
  *	遠隔操作で開くドア
  */
class RemoteDoor : public MapObject {
public:
	RemoteDoor() = default;
	virtual ~RemoteDoor() = default;

public:
	//	初期化
	virtual void Awake() override {
		GameObject* owner = GetOwner();
		Engine* engine = owner->GetEngine();

		owner->staticMesh = engine->GetStaticMesh(
			"Res/MeshData/door/door.obj"
		);
		auto collider = owner->AddComponent<AABBCollider>();
		collider->aabb = {
			{ -1.0f, 0.0f, -0.5f },
			{  1.0f, 2.0f,  0.5f }
		};
		if (owner->rotation.y == 90.0f * Deg2Rad ||
			owner->rotation.y == 270.0f * Deg2Rad) {
			collider->aabb = {
				{ -0.5f, 0.0f, -1.0f },
				{  0.5f, 2.0f,  1.0f }
			};
		}

		collider->isStatic = true;

		auto arch = engine->Create<GameObject>(
			owner->name, owner->position, owner->rotation
		);
		arch->staticMesh = engine->GetStaticMesh(
			"Res/MeshData/door/arch.obj"
		);
	}

	//	ドアを開ける
	virtual void RemoteAction(MapObject* linkedObject) override {
		//	ドアを床下に移動
		GetOwner()->position.y -= 2;
	}
};

#endif // !_REMOTEDOOR_H_
