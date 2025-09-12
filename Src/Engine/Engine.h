/*
 *	@file	Engine.h
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>

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
	//vbo
	//ibo
	//vao
	//tex

public:
	Engine() = default;
	~Engine() = default;

	int Run();
};

#endif // !_ENGINE_H_

