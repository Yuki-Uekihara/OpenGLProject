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
private:
	float startRotX = 0;	//	回転の開始角
	float endRotX = 0;		//	回転の終了角
	float currentTime = 0;	//	回転の経過時間
	float endTime = 0;		//	回転の終了時間

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

	//	更新
	virtual void Update(float deltaTime) override {
		//	レバーを目標の回転角に向かって回転させる
		if (currentTime < endRotX) {
			currentTime += deltaTime;

			if (currentTime >= endTime)
				currentTime = endTime;

			const float t = currentTime / endTime;
			GetOwner()->rotation.x =
				startRotX + (endRotX - startRotX) * t;
		}
	}

	//	クリック時に実行される
	virtual ActionResult Action() override {
		//	レバーの回転角を設定する
		startRotX = GetOwner()->rotation.x;
		endRotX = 60.0f * Deg2Rad;
		currentTime = 0;
		endTime = 1.0f;

		return ActionResult::executeRemoteAction;
	}
};

#endif // !_SIMPLELEVER_H_
