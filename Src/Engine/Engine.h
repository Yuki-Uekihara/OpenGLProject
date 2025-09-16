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

};

#endif // !_ENGINE_H_

