/*
 *	@file	Main.cpp
 */

#include <glad/glad.h>
#include "Engine/Engine.h"
#include "Engine/GameObject.h"

#include "TitleScene.h"
#include "MainGameScene.h"

#include <GLFW/glfw3.h>
#include <Windows.h>


/*
 *	エントリーポイント
 */
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
	Engine engine;
	engine.SetNextScene<TitleScene>();
	return engine.Run();
}

