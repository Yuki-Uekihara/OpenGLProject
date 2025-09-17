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

	GameObject camera;
	GameObject box0;
	GameObject box1;

	GLsizei indexCount = 0;

public:
	Engine() = default;
	~Engine() = default;

	int Run();

private:
	int Initialize();
	void Update();
	void Render();

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

};

#endif // !_ENGINE_H_

