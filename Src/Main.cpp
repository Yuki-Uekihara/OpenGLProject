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
 *	テクスチャを読み込み
 *	@param	filename
 *	@return	テクスチャの管理番号
 */
GLuint LoadTexture(const char* filename) {
	//	ファイルを開く
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file) {
		return 1;	//	失敗
	}

	//	ファイルのサイズを取得
	const size_t fileSize = std::filesystem::file_size(filename);
	//	ファイルのサイズ分の領域を確保
	std::vector<uint8_t> buffer(fileSize);
	//	読み込み + コピー
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

	//	ファイルを閉じる
	file.close();

	//	TGAヘッダから情報を取得
	const size_t tgaHeaderSize = 18;	//	ヘッダー情報のバイト数
	const int width = buffer[12] + buffer[13] * 256;
	const int height = buffer[14] + buffer[15] * 256;

	//	テクスチャを作成
	GLuint object = 0;		//	管理番号
	glCreateTextures(GL_TEXTURE_2D, 1, &object);
	glTextureStorage2D(object, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(
		object,
		0,
		0, 0,
		width, height,
		GL_BGRA,
		GL_UNSIGNED_BYTE,
		buffer.data() + tgaHeaderSize
	);

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

	//	Vector2
	struct Vector2 { float x, y; };
	//	Vector3
	struct Vector3 { float x, y, z; };
	//	頂点情報
	struct Vertex {
		Vector3 position;		//	頂点座標
		Vector2 texCoord;		//	テクスチャ座標
	};

	//	頂点データ
	const Vertex vertexData[] = {
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } },
	};

	//	頂点バッファの管理番号
	GLuint vbo = 0;		//	vertex buffer object
	//	バッファの作成
	glCreateBuffers(1, &vbo);
	//	GPUメモリを確保しデータをコピーする
	glNamedBufferStorage(vbo, sizeof(vertexData), vertexData, 0);


	//	インデックスデータ
	const GLushort indexData[] = {
		0, 1, 2, 2, 3, 0
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//	1番目の頂点属性(Vertex Attribute)を有効化
	glEnableVertexAttribArray(1);
	//	1番目の頂点属性を設定
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<const void*>(offsetof(Vertex, texCoord)));

	//	テクスチャを作成
	GLuint tex = LoadTexture("Res/box.tga");

	//	物体のパラメータ
	class GameObject {
	public:
		Vector3 position = { 0.0f, 0.0f, 0.0f };		//	平行移動
		Vector3 rotation = { 0.0f, 0.0f, 0.0f };		//	回転角度
		Vector3 scale = { 1.0f, 1.0f, 1.0f };			//	拡大率
		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//	色
	};

	GameObject box0;
	box0.scale = { 0.1f, 0.1f, 0.1f };
	box0.position = { -0.6f, -0.6f, -1.0f };


	GameObject box1;
	box1.color[1] = 0.5f;
	box1.scale = { 0.2f, 0.2f, 0.2f };
	box1.position = { 0.0f, 0.0f, -0.8f };

	//	メインループ
	while (!glfwWindowShouldClose(window)) {
		//	てすと
		box0.rotation.y += 0.1f;


		//	バックバッファをクリア
		glClearColor(0.9f, 0.6f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	描画に使うシェーダを指定
		glUseProgram(prog);

		//	ユニフォーム変数にデータをコピー
		//const float timer = static_cast<float>(glfwGetTime());
		//glProgramUniform1f(prog, 0, timer);
		glProgramUniform4fv(prog, 100, 1, box0.color);
		glProgramUniform3fv(prog, 0, 1, &box0.scale.x);
		glProgramUniform3fv(prog, 1, 1, &box0.position.x);
		glProgramUniform2f(prog, 2, sinf(box0.rotation.y), cosf(box0.rotation.y));


		//	描画に使うテクスチャをバインド
		glBindTextures(0, 1, &tex);

		//	図形を描画
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 1);

		glProgramUniform4fv(prog, 100, 1, box1.color);
		glProgramUniform3fv(prog, 0, 1, &box1.scale.x);
		glProgramUniform3fv(prog, 1, 1, &box1.position.x);
		glProgramUniform2f(prog, 2, sinf(box1.rotation.y), cosf(box1.rotation.y));

		//	図形を描画
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//	GLFWの終了
	glfwTerminate();

	return 0;
}