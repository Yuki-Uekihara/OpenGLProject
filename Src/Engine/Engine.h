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
 *	ゲームエンジン
 */
class Engine {
private:
	GLFWwindow* window = nullptr;				//	ウィンドウオブジェクト
	const std::string title = "OpenGLGame";		//	ウィンドウのタイトル
	GLuint vs = 0;
	GLuint fs = 0;
	GLuint prog = 0;
	GLuint vbo = 0;		//	vertex buffer object
	GLuint ibo = 0;		//	index buffer object
	GLuint vao = 0;		//	vertex array object
	GLuint tex = 0;

	GameObjectList gameObjects;		//	ゲームオブジェクトの一元管理

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
	 *	ゲームオブジェクトを生成する
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
		//	オブジェクトのポインタを生成
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position = position;
		p->rotation = rotation;
		//	一元管理に追加
		gameObjects.push_back(p);
		return p;
	}

};

#endif // !_ENGINE_H_

