/*
 *	@file	PlayerComponent.h
 */

#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#include "Engine/Component.h"
#include "Engine/Engine.h"

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
	virtual void Update(float deltaTime) override {
		Engine* engine = GetOwner()->GetEngine();
		GameObject& camera = engine->GetMainCamera();

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
