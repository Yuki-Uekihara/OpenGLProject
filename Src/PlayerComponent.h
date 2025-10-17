/*
 *	@file	PlayerComponent.h
 */

#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#include "Engine/Component.h"
#include "Engine/Engine.h"

/*
 *	�v���C���[
 */
class PlayerComponent : public Component {
public:
	//	�v���C���[�̏��
	enum class State {
		Alive,		//	�����Ă���
		Goal,		//	�S�[������
		Dead,		//	����ł���
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

		//	Alive�ȊO�̏�Ԃł͏������Ȃ�
		if (state != State::Alive)
			return;


		//	�J�����̈ړ�
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

		//	�e�X�g�J�����̉�]
		if (engine->GetKey(GLFW_KEY_LEFT)) {
			camera.rotation.y -= 0.05f;
		}
		if (engine->GetKey(GLFW_KEY_RIGHT)) {
			camera.rotation.y += 0.05f;
		}
	}

	//	�Փˎ��̏���
	virtual void OnCollision(const ComponentPtr& self, const ComponentPtr& other) override {
		//	�����Ă����ԂłȂ���Ώ������Ȃ�
		if (state != State::Alive)
			return;

		//	���������I�u�W�F�N�g�ŏ�Ԃ�J��
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
