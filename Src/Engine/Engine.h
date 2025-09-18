/*
 *	@file	Engine.h
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>

#include "GameObject.h"

/*
 *	�Q�[���G���W��
 */
class Engine {
private:
	GLFWwindow* window = nullptr;				//	�E�B���h�E�I�u�W�F�N�g
	const std::string title = "OpenGLGame";		//	�E�B���h�E�̃^�C�g��
	GLuint vs = 0;
	GLuint fs = 0;
	GLuint prog = 0;
	GLuint vbo = 0;		//	vertex buffer object
	GLuint ibo = 0;		//	index buffer object
	GLuint vao = 0;		//	vertex array object
	GLuint tex = 0;

	GameObjectList gameObjects;		//	�Q�[���I�u�W�F�N�g�̈ꌳ�Ǘ�
	float previousTime = 0;			//	�O��̍X�V��
	float deltaTime = 0;			//	�O��̍X�V����̌o�ߎ���


	GameObject camera;

	GLsizei indexCount = 0;

public:
	Engine() = default;
	~Engine() = default;

	int Run();

private:
	int Initialize();
	void Update();
	void Render();

	void UpdateGameObject(float deltaTime);
	void RemoveGameObject();
public:
	/*
	 *	�Q�[���I�u�W�F�N�g�𐶐�����
	 *	@param	name
	 *	@param	position
	 *	@param	rotation
	 */
	template <class T>
	std::shared_ptr<T> Create(
		const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f }
	) {
		//	�I�u�W�F�N�g�̃|�C���^�𐶐�
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position = position;
		p->rotation = rotation;
		//	�ꌳ�Ǘ��ɒǉ�
		gameObjects.push_back(p);
		return p;
	}	

	//	���ׂẴQ�[���I�u�W�F�N�g���폜����
	void ClearGameObjects();

public:
	//	�J�������擾����
	inline GameObject& GetMainCamera() { return camera; }
	inline const GameObject& GetMainCamera() const { return camera; }

	//	�L�[���͂̌��m
	inline bool GetKey(int key) const {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

};

#endif // !_ENGINE_H_

