/*
 *	@file	Engine.cpp
 */

#include "Engine.h"
#include "EasyAudio/EasyAudio.h"

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

//	必要な音声データ
//	BGM　2つ　	タイトル、げーむ中
//	SE　5つ		ボタンクリック音、ドア開く音、レバー動作音、
//				ゴールした音、敵に接触音
//	著作権表記テキスト残しておく。どのサイト、曲の名前。
// 
//	BGM
//	国内：最大規模の投稿サイト　dova-s.jp
//	海外：世界最大規模の投稿サイト　soundcloud.com
//	
//	SE
//	freesound.org	ポンぺウ・ファブラ大学の音楽技術グループ運営の音声投稿サイト

/*
 *	ゲームエンジンを実行する
 *	@return	0	正常
 */
int Engine::Run() {
	const int result = Initialize();
	if (result)
		return result;

	//	音声ライブラリの初期化
	if (!EasyAudio::Initialize())
		return -1;		//	初期化失敗

	while (!glfwWindowShouldClose(window)) {
		Update();
		Render();
		RemoveGameObject();

		//	音声ライブラリの更新
		EasyAudio::Update();
	}

	//	音声ライブラリの終了
	EasyAudio::Finalize();

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

	//	ライトの初期化
	InitializeLight();

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
		//	法線を設定するために図形のコピーを作る
		auto pVertex = static_cast<const Vertex*>(mesh.vertexData);
		auto pIndex = static_cast<const uint16_t*>(mesh.indexData);
		std::vector<Vertex> copyVertex(pVertex, pVertex + mesh.vertexSize / sizeof(Vertex));

		//	コピーした図形に法線を設定
		for (auto& v : copyVertex) {
			v.normal = Vector3::zero;
		}
		FillMissingNormals(copyVertex.data(), copyVertex.size(),
			pIndex, mesh.indexSize / sizeof(uint16_t));

		//	法線を設定した図形データをGPUメモリにコピーする
		meshBuffer->AddVertexData(copyVertex.data(), mesh.vertexSize,
			pIndex, mesh.indexSize);
	}

	//	OBJファイルの読み込み
	meshBuffer->LoadOBJ("Res/MeshData/skull/skull_highpoly_with_normal.obj");
	meshBuffer->LoadOBJ("Res/MeshData/ghost/ghost.obj");
	//meshBuffer->LoadOBJ("Res/MeshData/door/door.obj");
	//meshBuffer->LoadOBJ("Res/MeshData/door/arch.obj");
	meshBuffer->LoadOBJ("Res/MeshData/lever/lever_arm.obj");
	meshBuffer->LoadOBJ("Res/MeshData/lever/lever_cover.obj");
	meshBuffer->LoadOBJ("Res/MeshData/AlchemistHouse/BOX.obj");
	meshBuffer->LoadOBJ("Res/MeshData/AlchemistHouse/Door.obj");
	meshBuffer->LoadOBJ("Res/MeshData/AlchemistHouse/Arch.obj");
	meshBuffer->LoadOBJ("Res/MeshData/AlchemistHouse/Ceil.obj");
	meshBuffer->LoadOBJ("Res/MeshData/AlchemistHouse/Floor.obj");
	meshBuffer->LoadOBJ("Res/MeshData/HorrorHospital/Lamp.obj");

	//	一元管理配列の容量を予約
	gameObjects.reserve(1000);

	//	カメラの設定
	camera.position = { 3, 1, 3 };
	camera.rotation.y = 180.0f * Deg2Rad;

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

	//	マウスボタンの状態を取得
	for (size_t i = 0; i < std::size(mouseButtons); i++) {
		MouseButton& b = mouseButtons[i];
		b.prev = b.current;
		b.current = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT + i);

		//	押されている時間を更新
		if (b.prev) {
			b.timer += deltaTime;
		}
		else {
			b.timer = 0.0f;
		}

		//	クリックの判定
		//	前 && !今 && timer <= 0.3f
		if (b.prev && !b.current && b.timer <= mouseClickSpeed) {
			b.click = true;
		}
		else {
			b.click = false;
		}

	}

	

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

	UpdateShaderLight();

	//	ゲームオブジェクトをレンダーキュー順にソート
	std::stable_sort(
		gameObjects.begin(), gameObjects.end(),
		[](const GameObjectPtr& a, const GameObjectPtr& b) { return a->renderQueue < b->renderQueue; }
	);

	//	transparentキューの先頭を検索
	const auto transparentBegin = std::lower_bound(
		gameObjects.begin(), gameObjects.end(),
		RenderQueue_tranparent,
		[](const GameObjectPtr& obj, int v) { return obj->renderQueue < v; }
	);

	//	overlayキューの先頭を検索
	//	lower_bound 「検索条件を満たさない最初の要素のイテレータ」を返す
	const auto overlayBegin = std::lower_bound(
		transparentBegin, gameObjects.end(),		//	範囲の先頭, 範囲の終端
		RenderQueue_overlay,						//	検索する値
		[](const GameObjectPtr& obj, int v) {		//	検索する条件
			return obj->renderQueue < v;
		}
	);

	//	transparent以前の描画
	DrawGameObject(gameObjects.begin(), transparentBegin);

	//	transparentからoverlayまでのキューの描画
	glDepthMask(GL_FALSE);		//	深度バッファの書き込みを禁止
	DrawGameObject(transparentBegin, overlayBegin);
	glDepthMask(GL_TRUE);		//	深度バッファの書き込みを許可


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
		if (!obj->staticMesh && (obj->meshId < 0 || obj->meshId >= meshBuffer->GetDrawParamSize()))
			continue;

		glProgramUniform4fv(prog, 100, 1, obj->color);
		glProgramUniform3fv(prog, 0, 1, &obj->scale.x);
		glProgramUniform3fv(prog, 1, 1, &obj->position.x);
		glProgramUniform4f(prog, 2,
			sinf(obj->rotation.x), cosf(obj->rotation.x),
			sinf(obj->rotation.y), cosf(obj->rotation.y)
		);

		//	描画に使うテクスチャをバインド
		if (obj->texColor) {
			const GLuint tex = *obj->texColor;
			glBindTextures(0, 1, &tex);
		}

		//	図形を描画
		if (obj->staticMesh) {
			Draw(*obj->staticMesh, prog);
		}
		else {
			const DrawParam& param = meshBuffer->GetDrawParam(obj->meshId);
			glDrawElementsInstancedBaseVertex(
				param.mode, param.count, GL_UNSIGNED_SHORT, param.indices, 1, param.baseVertex);
		}
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
	for (auto& colA : *a) {
		for (auto& colB : *b) {
			//	スタティックなコライダー同士では処理しない
			if (colA.origin->isStatic && colB.origin->isStatic)
				continue;

			//	衝突判定
			Vector3 penetration;
			if (Intersect(colA.world, colB.world, penetration)) {
				GameObject* objA = colA.origin->GetOwner();
				GameObject* objB = colB.origin->GetOwner();

				//	コライダーが重ならないように座標を調整
				if (!colA.origin->isTrigger && !colB.origin->isTrigger) {
					//	Aがstaticか
					if (colA.origin->isStatic) {
						//	Bを調整
						colB.AddPosition(penetration);
					}
					//	Bがstaticか
					else if (colB.origin->isStatic) {
						//	Aを調整
						colA.AddPosition(-penetration);
					}
					//	どちらもstatic
					else {
						//	AもBも均等に調整
						colB.AddPosition(penetration * 0.5f);
						colA.AddPosition(-penetration);
					}
				}

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
 *	カメラに近いライトを選択してGPUのメモリにコピーする
 */
void Engine::UpdateShaderLight() {
	//	コピーするライトがなければライト数を０に設定する
	if (usedLight.empty()) {
		glProgramUniform1i(prog, locLightColorAndFallOffAngle, 0);
		return;
	}

	//	使用中のライト配列から、未使用になったライトを除外する
	const auto itrUnused = std::remove_if(
		usedLight.begin(), usedLight.end(),
		[this](int i) { return !lights[i].used; }
	);
	usedLight.erase(itrUnused, usedLight.end());

	//	重複する番号を除外する
	std::sort(usedLight.begin(), usedLight.end());
	const auto itrUnique = std::unique(usedLight.begin(), usedLight.end());
	usedLight.erase(itrUnique, usedLight.end());

	//	カメラの正面ベクトルを計算する
	const Vector3 forward = {
		-std::sinf(camera.rotation.y), 0.0f, -std::cosf(camera.rotation.y)
	};

	//	カメラからライトまでの距離を計算する
	struct Distance {
		float distance;			//	カメラからライトの半径までの距離
		const LightData* p;	//	ライトのアドレス
	};
	std::vector<Distance> distanceList;
	distanceList.reserve(lights.size());

	for (auto index : usedLight) {
		const auto& light = lights[index];
		const Vector3 v = light.position - camera.position;

		//	カメラの後ろにあるライトは無視
		float dot = forward.x * v.x + forward.y * v.y + forward.z * v.z;
		if (dot <= -light.radius)
			continue;

		//	カメラからライトの半径までの距離
		const float d = v.Magnitude() - light.radius;
		distanceList.push_back({ d, &light });
	}

	//	画面に影響するライトがなければライト数を０に設定する
	if (distanceList.empty()) {
		glProgramUniform1i(prog, locLightColorAndFallOffAngle, 0);
		return;
	}

	//	カメラに近いライトを優先する
	std::stable_sort(
		distanceList.begin(), distanceList.end(),
		[&](const auto& a, const auto& b) { return a.distance < b.distance; }
	);

	//	ライトデータをGPUメモリにコピーする
	const int lightCount = static_cast<int>(
		std::min(distanceList.size(), maxShaderLightCount)
		);
	std::vector<Vector4> colorAndFalloffAngle(lightCount);
	std::vector<Vector4> positionAndRadius(lightCount);
	std::vector<Vector4> directionAndConeAngle(lightCount);

	for (int i = 0; i < lightCount; i++) {
		const LightData* p = distanceList[i].p;
		const Vector3 color = p->color * p->intensity;
		colorAndFalloffAngle[i] = { color.x, color.y, color.z, p->falloffAngle };
		positionAndRadius[i] = { p->position.x, p->position.y, p->position.z, p->radius };
		directionAndConeAngle[i] = { p->direction.x, p->direction.y, p->direction.z, p->coneAngle };

		glProgramUniform4fv(prog, locLightColorAndFallOffAngle, lightCount, &colorAndFalloffAngle[0].x);
		glProgramUniform4fv(prog, locLightPositionAndRadius, lightCount, &positionAndRadius[0].x);
		glProgramUniform4fv(prog, locLightDirectionAndConeAngle, lightCount, &directionAndConeAngle[0].x);
		glProgramUniform1i(prog, locLightCount, lightCount);
	}
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

/*
 *	マウス座標から発射される光線を取得する
 */
Ray Engine::GetRayFromMousePosition() const {
	//	スクリーン座標系のマウスカーソル座標を取得する
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	//	スクリーン座標系からNDC座標系に変換する
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	Vector3 nearPos = {
		static_cast<float>(x / w * 2 - 1),
		-static_cast<float>(y / h * 2 - 1),
		-1
	};
	Vector3 farPos = { nearPos.x, nearPos.y, 1 };

	//	深度値の計算結果が -1 ~ +1 になるようなパラメータA,Bを計算する
	//	頂点シェーダの値と一致させる必要がある
	const float near = 0.35f;
	const float far = 1000.0f;
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);

	// NDC座標系からクリップ座標系に変換する
	nearPos *= near;
	farPos *= far;
	nearPos.z = (nearPos.x - 1) / B;
	farPos.z = (farPos.z - A) / B;

	//	アスペクト比とFoVを計算する
	const float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
	const float invFovScale = 1.0f / GetFovScale();

	//	クリップ座標系からビュー座標系に変換する
	nearPos.x *= invFovScale * aspectRatio;
	nearPos.y *= invFovScale;
	farPos.x *= invFovScale * aspectRatio;
	farPos.y *= invFovScale;

	//	ビュー座標系からワールド座標系に変換する
	const float cameraSinY = std::sinf(camera.rotation.y);
	const float cameraCosY = std::cosf(camera.rotation.y);

	nearPos = {
		nearPos.x * cameraCosY - near * cameraSinY,
		nearPos.y,
		nearPos.x * -cameraSinY - near * cameraCosY
	};
	nearPos += camera.position;

	farPos = {
		farPos.x * cameraCosY - far * cameraSinY,
		farPos.y,
		farPos.x * -cameraSinY - far * cameraCosY
	};
	farPos += camera.position;

	//	光線の方向ベクトルを求める
	Vector3 direction = farPos - nearPos;
	direction = direction.Normalized();

	return Ray{ nearPos, direction };
}

/*
 *	光線とコライダーの交差判定
 *	@param	ray			光線
 *	@param	hitInfo		光線と最初に交差したコライダーの情報
 *	@param	pred		交差判定を行うコライダーを選別する情報（述語）
 *	@return	bool		交差しているかどうか
 */
bool Engine::Raycast(const Ray& ray, RaycastHit& hitInfo, const RaycastPredicate& pred) const {
	//	交点の情報を初期化
	hitInfo.collider = nullptr;
	hitInfo.distance = FLT_MAX;

	for (const auto& obj : gameObjects) {
		for (const auto& col : obj->colliders) {
			//	AABBをワールド座標系に変換
			const AABB worldAABB = {
				Vector3::Scale(col->aabb.min, obj->scale) + obj->position,
				Vector3::Scale(col->aabb.max, obj->scale) + obj->position
			};

			//	光線との交差判定
			float d;
			if (!Intersect(worldAABB, ray, d))
				continue;

			//	交差判定の対象かどうか
			if (!pred(col, d))
				continue;

			//	最初に当たったコライダーを保存
			if (d < hitInfo.distance) {
				hitInfo.collider = col;
				hitInfo.distance = d;
			}
		}
	}

	//	交差するコライダーがある場合
	if (hitInfo.collider) {
		//	交点の座標を計算
		hitInfo.point = ray.origin + ray.direction * hitInfo.distance;
		return true;
	}

	return false;
}

/*
 *	ライト配列を初期化する
 */
void Engine::InitializeLight() {
	//	指定されたライトの数を生成
	lights.resize(lightResizeCount);
	usedLight.reserve(lightResizeCount);
	freeLight.resize(lightResizeCount);

	//	全てのライトを未使用ライト配列に「逆順」で追加
	for (int i = 0; i < lightResizeCount; i++) {
		freeLight[i] = static_cast<int>(lightResizeCount - i - 1);
	}
}

/*
 *	新しいライトの取得
 *	@return	ライトのインデックス
 */
int Engine::AllocateLight() {
	//	未使用のライトがなければライト配列を拡張する
	if (freeLight.empty()) {
		const size_t oldSize = lights.size();
		lights.reserve(oldSize + lightResizeCount);
		//	拡張した分を「逆順」で未使用ライト配列に追加
		for (size_t i = lights.size() - 1; i >= oldSize; i--) {
			freeLight.push_back(static_cast<int>(i));
		}
	}

	//	未使用ライト配列の末尾からインデックスを取り出す
	const int index = freeLight.back();
	freeLight.pop_back();

	//	取り出したインデックスを使用中ライト配列に追加
	usedLight.push_back(index);
	//	そのインデックスのライトを使用中に変更する
	lights[index].used = true;

	return index;
}

/*
 *	ライトの解放
 *	@param	ライトのインデックス
 */
void Engine::DeallocateLight(int index) {
	if (index >= 0 && index < lights.size()) {
		//	インデックスを未使用ライト配列に追加
		freeLight.push_back(index);
		//	そのインデックスのライトを未使用に変更する
		lights[index].used = false;
	}
}
