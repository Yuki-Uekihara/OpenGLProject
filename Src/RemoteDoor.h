/*
 *	@file	RemoteDoor.h
 */

#ifndef _REMOTEDOOR_H_
#define	_REMOTEDOOR_H_

#include "MapObject.h"
#include "Engine/Engine.h"
#include "Engine/AABBCollider.h"

 /*
  *	遠隔操作で開くドア
  */
class RemoteDoor : public MapObject {
private:
	bool isActioned = false;

public:
	RemoteDoor() = default;
	virtual ~RemoteDoor() = default;

public:
	//	初期化
	virtual void Awake() override {
		GameObject* owner = GetOwner();
		Engine* engine = owner->GetEngine();

		owner->staticMesh = engine->GetStaticMesh("Res/MeshData/AlchemistHouse/Door.obj");
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
		arch->staticMesh = engine->GetStaticMesh("Res/MeshData/AlchemistHouse/Arch.obj");

		//	枠をドアの親にする
		owner->SetParent(arch);
		owner->position = { -0.4f, 0.0f, 0.0f };
		owner->rotation = { 0.0f, 0.0f, 0.0f };

	}

	virtual void Update(float deltaTime) override {
		if (!isActioned)
			return;

		constexpr float speed = 120.0f * Deg2Rad;
		GameObject* owner = GetOwner();

		//	90度になるまで回転
		owner->rotation.y += speed * deltaTime;
		if (owner->rotation.y >= 90.0f * Deg2Rad)
			owner->rotation.y = 90.0f * Deg2Rad;

		auto col = owner->GetComponent<AABBCollider>();
		if (col)
			col->isTrigger = true;
	}

	//	ドアを開ける
	virtual void RemoteAction(MapObject* linkedObject) override {
		isActioned = true;
	}
};

#endif // !_REMOTEDOOR_H_
