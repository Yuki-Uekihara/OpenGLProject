/*
 *	@file	Engine.h
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>

/*
 *	ゲームエンジン
 */
class Engine {
private:
	GLFWwindow* window = nullptr;				//	ウィンドウオブジェクト
	const std::string title = "OpenGLGame";		//	ウィンドウのタイトル
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

