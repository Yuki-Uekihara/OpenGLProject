/*
 *	@file	NormalDoor.cpp
 */

#include "NormalDoor.h"
#include "Engine/Engine.h"
#include "Engine/EasyAudio/EasyAudio.h"
#include "AudioSettings.h"

//	ゲームオブジェクトに追加されたときに呼び出される
void NormalDoor::Awake() {
	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	//	ドアの枠
	owner->staticMesh = engine->GetStaticMesh(
		"Res/MeshData/AlchemistHouse/Arch.obj"
	);


	//	ドア
	door = engine->Create<GameObject>(owner->name + ".Door", { -0.4f, 0.0f, 0.0f });
	door->staticMesh = engine->GetStaticMesh(
		"Res/MeshData/AlchemistHouse/Door.obj"
	);
	door->SetParent(owner);

	//	角度に対応するコライダーの形状を設定
	collider = owner->AddComponent<AABBCollider>();
	collider->isStatic = true;
	if (owner->rotation.y == 90.0f * Deg2Rad) {
		collider->aabb = {
			{ -0.5f, 0.0f, -1.0f },
			{  0.5f, 2.0f,  1.0f }
		};
	}
	else {
		collider->aabb = {
			{ -1.0f, 0.0f, -0.5f },
			{  1.0f, 2.0f,  0.5f }
		};
	}
}

void NormalDoor::Update(float deltaTime) {
	constexpr float speed = 120.0f * Deg2Rad;

	switch (state) {
	case NormalDoor::State::Opening:
		//	90度になるまで徐々に回転させる
		door->rotation.y += speed * deltaTime;

		if (door->rotation.y >= 90.0f * Deg2Rad) {
			door->rotation.y = 90.0f * Deg2Rad;
			state = State::Open;
		}
		break;

	case NormalDoor::State::Closed:
	case NormalDoor::State::Open:
	case NormalDoor::State::Closing:
		break;
	}
}

MapObject::ActionResult NormalDoor::Action() {
	switch (state) {
	case NormalDoor::State::Closed:
	case NormalDoor::State::Closing:
		//	コライダーを通過できるようにする
		collider->isTrigger = true;
		EasyAudio::Play(1, SE::doorOpen);
		state = State::Opening;
		break;

	case NormalDoor::State::Opening:
	case NormalDoor::State::Open:
		break;
	}

	return ActionResult::executeRemoteAction;
}
