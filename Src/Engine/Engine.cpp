/*
 *	@file	Engine.cpp
 */

#include "Engine.h"
#include <fstream>
#include <filesystem>
#include <vector>

#include "../PlayerComponent.h"

 //	図形データ
#include "../../Res/MeshData/crystal_mesh.h"
#include "../../Res/MeshData/wall_mesh.h"
#include "../../Res/MeshData/plane_xy_mesh.h"
#include "../../Res/MeshData/skull_mesh.h"


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
		RemoveGameObject();
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

	//	図形データの情報
	struct MeshData {
		size_t vertexSize;			//	頂点データのバイト数
		size_t indexSize;			//	インデックスデータのバイト数
		const void* vertexData;		//	頂点データのアドレス
		const void* indexData;		//	インデックスデータのアドレス
	};

	const MeshData meshes[] = {
		{ sizeof(vertexData), sizeof(indexData), vertexData, indexData },
		{ sizeof(crystal_vertices), sizeof(crystal_indices), crystal_vertices, crystal_indices },
		{ sizeof(wall_vertices), sizeof(wall_indices), wall_vertices, wall_indices },
		{ sizeof(plane_xy_vertices), sizeof(plane_xy_indices), plane_xy_vertices, plane_xy_indices },
		{ sizeof(skull_vertices), sizeof(skull_indices), skull_vertices, skull_indices },

	};


	//	図形データから描画パラメータを作成し、データをGPUメモリにコピーする
	meshBuffer = MeshBuffer::Create(32'000'000);
	for (const auto& mesh : meshes) {
		meshBuffer->AddVertexData(
			static_cast<const Vertex*>(mesh.vertexData), mesh.vertexSize,
			static_cast<const uint16_t*>(mesh.indexData), mesh.indexSize
		);
	}
	
	//	一元管理配列の容量を予約
	gameObjects.reserve(1000);

	//	カメラの設定
	camera.position = { 3, 1, 3 };
	camera.rotation.y = 3.14159265f;

	return 0;
}

/*
 *	ゲームエンジンの状態を更新する
 */
void Engine::Update() {

	//	デルタタイムの計算
	const float currentTime = static_cast<float>(glfwGetTime());	//	現在
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	//	デバッグ用
	if (deltaTime >= 0.5f)
		deltaTime = 1.0f / 60.0f;

	//	シーンの切り替え
	if (nextScene) {
		if (scene)
			scene->Finalize(*this);

		nextScene->Initialize(*this);
		scene = std::move(nextScene);
	}

	//	シーンの更新
	if (scene)
		scene->Update(*this, deltaTime);


	UpdateGameObject(deltaTime);
	HandleGameObjectCollision();
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
	//	ユニフォーム変数にデータをコピー
	//const float timer = static_cast<float>(glfwGetTime());
	//glProgramUniform1f(prog, 0, timer);
	glProgramUniform2f(prog, 3, fovScale / aspectRatio, fovScale);
	glProgramUniform3fv(prog, 4, 1, &camera.position.x);
	glProgramUniform2f(prog, 5, sinf(-camera.rotation.y), cosf(-camera.rotation.y));

	//	深度テストの有効化
	glEnable(GL_DEPTH_TEST);

	//	半透明合成の有効化
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	//	ゲームオブジェクトをレンダーキュー順にソート
	std::stable_sort(
		gameObjects.begin(), gameObjects.end(),
		[](const GameObjectPtr& a, const GameObjectPtr& b) { return a->renderQueue < b->renderQueue; }
	);

	//	overlayキューの先頭検索
	//	lower_bound 「検索条件を満たさない最初の要素のイテレータ」を返す
	const auto overlayBegin = std::lower_bound(
		gameObjects.begin(), gameObjects.end(),		//	範囲の先頭, 範囲の終端
		RenderQueue_overlay,						//	検索する値
		[](const GameObjectPtr& obj, int v) {		//	検索する条件
			return obj->renderQueue < v;
		}										
	);

	//	overlay以前の描画
	DrawGameObject(gameObjects.begin(), overlayBegin);

	//	overlay以降の描画
	glDisable(GL_DEPTH_TEST);
	DrawGameObject(overlayBegin, gameObjects.end());



	glfwSwapBuffers(window);
	glfwPollEvents();
}

/*
 *	ゲームオブジェクト配列を描画する 
 */
void Engine::DrawGameObject(GameObjectList::iterator begin, GameObjectList::iterator end) {
	//	メッシュバッファからVAOをバインド
	glBindVertexArray(*meshBuffer->GetVAO());
	
	for (GameObjectList::iterator i = begin; i != end; ++i) {
		const auto& obj = *i;

		//	図形番号が対象外の場合は描画しない
		if (obj->meshId < 0 || obj->meshId >= meshBuffer->GetDrawParamSize())
			continue;

		glProgramUniform4fv(prog, 100, 1, obj->color);
		glProgramUniform3fv(prog, 0, 1, &obj->scale.x);
		glProgramUniform3fv(prog, 1, 1, &obj->position.x);
		glProgramUniform2f(prog, 2, sinf(obj->rotation.y), cosf(obj->rotation.y));

		//	描画に使うテクスチャをバインド
		if (obj->texColor) {
			const GLuint tex = *obj->texColor;
			glBindTextures(0, 1, &tex);
		}

		//	図形を描画
		const DrawParam& param = meshBuffer->GetDrawParam(obj->meshId);
		glDrawElementsInstancedBaseVertex(
			param.mode, param.count, GL_UNSIGNED_SHORT, param.indices, 1, param.baseVertex);
	}
	glBindVertexArray(0);
}

/*
 *	ゲームオブジェクトの状態を更新する
 *	@	param	deltaTime
 */
void Engine::UpdateGameObject(float deltaTime) {
	//	要素の追加に対応するために 普通の for文を選択
	for (int i = 0; i < gameObjects.size(); i++) {
		GameObject* pObj = gameObjects[i].get();
		if (!pObj->IsDestroyed()) {
			pObj->Start();
			pObj->Update(deltaTime);
		}
	}
}

/*
 *	ゲームオブジェクトの衝突を処理する
 */
void Engine::HandleGameObjectCollision() {
	//	ワールド座標系の衝突判定を作成
	std::vector<WorldColliderList> colliders;
	colliders.reserve(gameObjects.size());

	//	全てのオブジェクトでコライダーの座標を計算
	for (const auto obj : gameObjects) {
		//	コライダーを持っていないオブジェクトは処理しない
		if (obj->colliders.empty())
			continue;

		WorldColliderList list(obj->colliders.size());

		for (int i = 0; i < obj->colliders.size(); i++) {
			//	オリジナルのコライダーをコピー
			list[i].origin = obj->colliders[i];
			list[i].world = obj->colliders[i]->aabb;

			//	ローカル座標をワールド座標に変換
			list[i].world.min = Vector3::Scale(list[i].world.min, obj->scale);
			list[i].world.max = Vector3::Scale(list[i].world.max, obj->scale);
			list[i].world.min += obj->position;
			list[i].world.max += obj->position;
		}
		colliders.push_back(list);
	}

	if (colliders.size() >= 2) {
		//	ゲームオブジェクト毎の衝突判定
		for (auto a = colliders.begin(); a != colliders.end() - 1; a++) {
			const GameObject* objA = a->at(0).origin->GetOwner();
			//	削除済みは処理しない
			if (objA->IsDestroyed())
				continue;

			for (auto b = a + 1; b != colliders.end(); b++) {
				const GameObject* objB = b->at(0).origin->GetOwner();
				//	削除済みは処理しない
				if (objB->IsDestroyed())
					continue;

				//	コライダー単位の衝突判定
				HandleWorldColliderCollision(&(*a), &*b);
			}
		}
	}
}

/*
 *	コライダー単位の当たり判定
 *	@param	a	判定対象のコライダー配列1
 *	@param	b	判定対象のコライダー配列2
 */
void Engine::HandleWorldColliderCollision(WorldColliderList* a, WorldColliderList* b) {
	//	コライダー毎の衝突判定
	for (const auto& colA : *a) {
		for (const auto& colB : *b) {
			//	スタティックなコライダー同士では処理しない
			if (colA.origin->isStatic && colB.origin->isStatic)
				continue;

			//	衝突判定
			Vector3 penetration;
			if (Intersect(colA.world, colB.world, penetration)) {
				GameObject* objA = colA.origin->GetOwner();
				GameObject* objB = colB.origin->GetOwner();

				//	イベントの発火
				objA->OnCollision(colA.origin, colB.origin);
				objB->OnCollision(colB.origin, colA.origin);

				//	イベントの結果、どちらかが削除予定が入ったら処理を抜ける
				if (objA->IsDestroyed() || objB->IsDestroyed())
					return;
			}
		}
	}
}

/*
 *	破棄予定のゲームオブジェクトを削除する
 */
void Engine::RemoveGameObject() {
	//	ゲームオブジェクトがなければ処理しない
	if (gameObjects.empty())
		return;

	//	破棄の予定の有無でゲームオブジェクトを分ける
	const auto itr = std::stable_partition(
		gameObjects.begin(), gameObjects.end(),
		[](const auto& p) { return !p->IsDestroyed(); }
	);

	//	削除予定のゲームオブジェクトを別の配列に移動
	std::vector<GameObjectPtr> destroyList(
		std::move_iterator(itr),
		std::move_iterator(gameObjects.end())
	);

	//	配列から移動済みのゲームオブジェクトを削除
	gameObjects.erase(itr, gameObjects.end());

	//	破棄予定のゲームオブジェクトにイベント発火
	for (auto& c : destroyList) {
		c->OnDestroy();
	}

	//	ローカル変数 destroyList の寿命はここまで
}


/*
 *	ゲームエンジンから全てのゲームオブジェクトを破棄する
 */
void Engine::ClearGameObjects() {
	for (auto& obj : gameObjects) {
		obj->OnDestroy();
	}
	gameObjects.clear();
}