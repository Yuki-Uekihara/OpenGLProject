/*
 *	@file	Main.cpp
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>

/*
 *	シェーダーファイルを読み込み ＋ コンパイル
 *	@param	type
 *	@param	filename
 *	@return	シェーダの管理番号
 */
GLuint CompileShader(GLenum type, const char* filename) {
	//	ファイルを開く
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file) {
		return 1;	//	失敗
	}

	//	ファイルのサイズを取得
	const size_t fileSize = std::filesystem::file_size(filename);
	//	ファイルのサイズ分の領域を確保
	std::vector<char> buffer(fileSize);
	//	読み込み + コピー
	file.read(buffer.data(), fileSize);

	//	ファイルを閉じる
	file.close();

	const char* source[] = { buffer.data() };
	const GLint length[] = { int(buffer.size()) };
	//	シェーダーを作成
	const GLuint object = glCreateShader(type);
	//	シェーダのソースを設定
	glShaderSource(object, 1, source, length);
	//	シェーダをコンパイル
	glCompileShader(object);

	return object;
}

/*
 *	エントリーポイント
 */
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {

	//	GLFWの初期化
	if (glfwInit() != GLFW_TRUE)
		return 1;	//	失敗

	//	描画ウィンドウの作成
	GLFWwindow* window = nullptr;				//	ウィンドウオブジェクト
	const std::string title = "OpenGLGame";		//	ウィンドウのタイトル
	window = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		return 1;	//	失敗
	}

	//	OpenGLコンテキストの作成
	glfwMakeContextCurrent(window);

	//	OpenGL関数のアドレスを取得
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		glfwTerminate();
		return 1;	//	失敗
	}

	//	シェーダーを読み込んでコンパイル
	const GLuint vs = CompileShader(GL_VERTEX_SHADER, "Res/standard.vert");
	const GLuint fs = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");

	//	シェーダーをリンク
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	//	頂点データ
	const float vertexData[][3] = {
		{ -0.2f, -0.2f, 0.0f },
		{  0.2f, -0.2f, 0.0f },
		{  0.0f,  0.2f, 0.0f },

		{ -0.8f, -0.2f, 0.0f },
		{ -0.4f, -0.2f, 0.0f },
		{ -0.4f,  0.2f, 0.0f },
		{ -0.8f,  0.2f, 0.0f },
	};

	//	頂点バッファの管理番号
	GLuint vbo = 0;		//	vertex buffer object
	//	バッファの作成
	glCreateBuffers(1, &vbo);
	//	GPUメモリを確保しデータをコピーする
	glNamedBufferStorage(vbo, sizeof(vertexData), vertexData, 0);


	//	インデックスデータ
	const GLushort indexData[] = {
		0, 1, 2,
		3, 4, 5,  5, 6, 3,
	};

	//	インデックスバッファの管理番号
	GLuint ibo = 0;		//	index buffer object
	//	バッファの作成
	glCreateBuffers(1, &ibo);
	//	GPUメモリを確保しデータをコピーする
	glNamedBufferStorage(ibo, sizeof(indexData), indexData, 0);


	//	頂点属性(Vertex Attribute)配列の管理番号
	GLuint vao = 0;		//	vertex array object
	//	VAO を生成
	glCreateVertexArrays(1, &vao);
	//	OpenGLコンテキスト にバインド
	glBindVertexArray(vao);

	//	IBO を OpenGLコンテキスト と VAO にバインド
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//	VBO を OpenGLコンテキスト にバインド
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	0番目の頂点属性(Vertex Attribute)を有効化
	glEnableVertexAttribArray(0);
	//	0番目の頂点属性を設定
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//	メインループ
	while (!glfwWindowShouldClose(window)) {
		//	バックバッファをクリア
		glClearColor(0.9f, 0.6f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	描画に使うシェーダを指定
		glUseProgram(prog);

		//	図形を描画
		glBindVertexArray(vao);
		glDrawElementsInstanced(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, 0, 1);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//	GLFWの終了
	glfwTerminate();

	return 0;
}