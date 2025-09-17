/*
 *	@file	Engine.h
 */

#include "Engine.h"
#include <fstream>
#include <filesystem>
#include <vector>

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
 *	ゲームエンジンを実行する
 *	@return	0	正常
 */
int Engine::Run() {
	const int result = Initialize();
	if (result)
		return result;

	while (!glfwWindowShouldClose(window)) {
		Update();
		Render();
	}

	glfwTerminate();
	return 0;
}

/*
 *	ゲームエンジンを初期化する
 *	@return		0 正常
 */
int Engine::Initialize() {
	//	GLFWの初期化
	if (glfwInit() != GLFW_TRUE)
		return 1;	//	失敗

	//	描画ウィンドウの作成
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
	vs = CompileShader(GL_VERTEX_SHADER, "Res/standard.vert");
	fs = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");

	//	シェーダーをリンク
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	//	頂点情報
	struct Vertex {
		Vector3 position;		//	頂点座標
		Vector2 texCoord;		//	テクスチャ座標
	};

	//	頂点データ
	const Vertex vertexData[] = {
		//	+Z 手前の面
		{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
		{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } },

		//	-Z 奥の面
		{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f } },

		//	+Y 上の面
		{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } },

		//	-Y 下の面
		{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },

		//	+X 右の面
		{ {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
		{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } },

		//	-X 左の面
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f } },
	};

	//	頂点バッファの管理番号
	vbo = 0;		//	vertex buffer object
	//	バッファの作成
	glCreateBuffers(1, &vbo);
	//	GPUメモリを確保しデータをコピーする
	glNamedBufferStorage(vbo, sizeof(vertexData), vertexData, 0);


	//	インデックスデータ
	const GLushort indexData[] = {
		  0,   1,   2,    2,   3,   0,		//	+Z
		  4,   5,   6,    6,   7,   4,		//	-Z
		  8,   9,  10,   10,  11,   8,		//	+Y
		 12,  13,  14,   14,  15,  12,		//	-Y
		 16,  17,  18,   18,  19,  16,		//	+X
		 20,  21,  22,   22,  23,  20,		//	-X
	};
	//	インデックスを取得
	indexCount = static_cast<GLsizei>(std::size(indexData));



	//	インデックスバッファの管理番号
	ibo = 0;		//	index buffer object
	//	バッファの作成
	glCreateBuffers(1, &ibo);
	//	GPUメモリを確保しデータをコピーする
	glNamedBufferStorage(ibo, sizeof(indexData), indexData, 0);


	//	頂点属性(Vertex Attribute)配列の管理番号
	vao = 0;		//	vertex array object
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
	tex = LoadTexture("Res/box.tga");

	//	一元管理配列の容量を予約
	gameObjects.reserve(1000);

	box0.scale = { 0.1f, 0.1f, 0.1f };
	box0.position = { -0.6f, -0.6f, -1.0f };

	box1.color[1] = 0.5f;
	box1.scale = { 0.2f, 0.2f, 0.2f };
	box1.position = { 0.0f, 0.0f, -0.8f };

	return 0;
}

/*
 *	ゲームエンジンの状態を更新する
 */
void Engine::Update() {
	//	テスト回転
	box0.rotation.y += 0.01f;

	//	カメラの移動
	const float cameraSpeed = 0.005f;
	const float cameraCos = cos(camera.rotation.y);
	const float cameraSin = sin(camera.rotation.y);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.position.x -= cameraSpeed * cameraCos;
		camera.position.z -= cameraSpeed * -cameraSin;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.position.x += cameraSpeed * cameraCos;
		camera.position.z += cameraSpeed * -cameraSin;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.position.x -= cameraSpeed * cameraSin;
		camera.position.z -= cameraSpeed * cameraCos;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.position.x += cameraSpeed * cameraSin;
		camera.position.z += cameraSpeed * cameraCos;
	}

	//	テストカメラの回転
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera.rotation.y -= 0.005f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera.rotation.y += 0.005f;
	}

}

/*
 *	ゲームエンジンの状態を描画する
 */
void Engine::Render() {
	//	バックバッファをクリア
	glClearColor(0.9f, 0.6f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	描画に使うシェーダを指定
	glUseProgram(prog);

	//	フレームバッファ(画面)の大きさを取得
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	//	ビューポートの設定
	glViewport(0, 0, fbWidth, fbHeight);

	//	アスペクト比を計算
	const float aspectRatio =
		static_cast<float>(fbWidth) / static_cast<float>(fbHeight);

	const float degFovY = 60;	//	垂直の視野角
	const float radFovY = degFovY * 3.141592f / 180.0f;
	const float scaleFov = tanf(radFovY / 2);	//	視野角による拡大率

	//	ユニフォーム変数にデータをコピー
	//const float timer = static_cast<float>(glfwGetTime());
	//glProgramUniform1f(prog, 0, timer);
	glProgramUniform4fv(prog, 100, 1, box0.color);
	glProgramUniform3fv(prog, 0, 1, &box0.scale.x);
	glProgramUniform3fv(prog, 1, 1, &box0.position.x);
	glProgramUniform2f(prog, 2, sinf(box0.rotation.y), cosf(box0.rotation.y));
	glProgramUniform2f(prog, 3, 1 / (aspectRatio * scaleFov), 1 / scaleFov);
	glProgramUniform3fv(prog, 4, 1, &camera.position.x);
	glProgramUniform2f(prog, 5, sinf(-camera.rotation.y), cosf(-camera.rotation.y));

	//	深度テストの有効化
	glEnable(GL_DEPTH_TEST);

	//	描画に使うテクスチャをバインド
	glBindTextures(0, 1, &tex);
	//	使用する頂点属性を割り当て
	glBindVertexArray(vao);

	//	図形を描画
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0, 1);

	glProgramUniform4fv(prog, 100, 1, box1.color);
	glProgramUniform3fv(prog, 0, 1, &box1.scale.x);
	glProgramUniform3fv(prog, 1, 1, &box1.position.x);
	glProgramUniform2f(prog, 2, sinf(box1.rotation.y), cosf(box1.rotation.y));

	//	図形を描画
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0, 1);

	glBindVertexArray(0);		//	VAOの割り当てを解除

	glfwSwapBuffers(window);
	glfwPollEvents();


}