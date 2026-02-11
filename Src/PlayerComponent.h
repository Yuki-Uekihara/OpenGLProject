/*
 *	@file	PlayerComponent.h
 */

#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/Light.h"

/*
 *	プレイヤー
 */
class PlayerComponent : public Component {
public:
	//	プレイヤーの状態
	enum class State {
		Alive,		//	生きている
		Goal,		//	ゴールした
		Dead,		//	死んでいる
	};

private:
	State state = State::Alive;

public:
	PlayerComponent() = default;
	virtual ~PlayerComponent() = default;

public:
	virtual void Awake() override {
		auto owner = GetOwner();
		auto engine = owner->GetEngine();

		//	手
		auto hand = engine->Create<GameObject>("player.hand", { 1.0f, -0.4f, -1.5f });
		hand->rotation.x = -35.0f * Deg2Rad;
		hand->rotation.y = 180.0f * Deg2Rad;
		hand->staticMesh = engine->GetStaticMesh("Res/MeshData/arm_and_hand/arm_and_hand_grab.obj");
		hand->SetParent(owner);

		//	武器
		auto weapon = engine->Create<GameObject>("weapon", { 0.0f, -0.35f, 0.05f });
		weapon->rotation.x = 0.0f * Deg2Rad;
		weapon->rotation.y = 35.0f * Deg2Rad;
		weapon->staticMesh = engine->GetStaticMesh("Res/MeshData/Weapon/sword.obj");
		weapon->SetParent(hand);

		auto light = owner->AddComponent<Light>();
		light->color = { 1.0f, 0.9f, 0.8f };
		light->intensity = 5.0f;
		light->radius = 5.0f;
	}

	virtual void Update(float deltaTime) override {
		Engine* engine = GetOwner()->GetEngine();
		GameObject& camera = *GetOwner();

		//	Alive以外の状態では処理しない
		if (state != State::Alive)
			return;


		//	カメラの移動
		const float cameraSpeed = 0.05f;
		const float cameraCos = cos(camera.rotation.y);
		const float cameraSin = sin(camera.rotation.y);

		if (engine->GetKey(GLFW_KEY_A)) {
			camera.position.x -= cameraSpeed * cameraCos;
			camera.position.z -= cameraSpeed * -cameraSin;
		}
		if (engine->GetKey(GLFW_KEY_D)) {
			camera.position.x += cameraSpeed * cameraCos;
			camera.position.z += cameraSpeed * -cameraSin;
		}
		if (engine->GetKey(GLFW_KEY_W)) {
			camera.position.x -= cameraSpeed * cameraSin;
			camera.position.z -= cameraSpeed * cameraCos;
		}
		if (engine->GetKey(GLFW_KEY_S)) {
			camera.position.x += cameraSpeed * cameraSin;
			camera.position.z += cameraSpeed * cameraCos;
		}

		//	テスト上下移動
		if (engine->GetKey(GLFW_KEY_Q)) {
			camera.position.y -= cameraSpeed;
		}
		if (engine->GetKey(GLFW_KEY_E)) {
			camera.position.y += cameraSpeed;
		}


		//	テストカメラの回転
		if (engine->GetKey(GLFW_KEY_LEFT)) {
			camera.rotation.y -= 0.05f;
		}
		if (engine->GetKey(GLFW_KEY_RIGHT)) {
			camera.rotation.y += 0.05f;
		}
	}

	//	衝突時の処理
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other) override {
		//	生きている状態でなければ処理しない
		if (state != State::Alive)
			return;

		//	当たったオブジェクトで状態を遷移
		const auto& name = other->GetOwner()->name;

		if (name == "enemy") {
			state = State::Dead;
		}
		else if (name == "goal") {
			state = State::Goal;
		}
	}

public:
	inline State GetState() const { return state; }

};

#endif // !_PLAYERCOMPONENT_H_
