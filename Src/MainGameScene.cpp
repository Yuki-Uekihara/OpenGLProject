/*
 *	@file	MainGameScene.cpp
 */

#include "MainGameScene.h"
#include "Engine/Engine.h"
#include "Engine/Collision.h"
#include "PlayerComponent.h"
#include "GoalEvent.h"
#include "EnemySkull.h"
#include "TitleScene.h"
#include "Engine/UIButton.h"

#include <fstream>
#include <string>



 /*
  *	シーンを初期化する
  *	@return	true	成功
  */
bool MainGameScene::Initialize(Engine& engine) {
	//	ファイルからデータを読み込む
	std::ifstream file("Res/maze00.txt");
	if (!file)
		return false;	//	失敗

	file >> mapSizeX >> mapSizeY;	//	マップの大きさを読み込む
	file.ignore();					//	改行を飛ばす

	//	データを読み込む
	map.resize(mapSizeX * mapSizeY);
	for (int y = 0; y < mapSizeY; y++) {
		std::string s;
		std::getline(file, s);

		for (int x = 0; x < mapSizeX; x++) {
			GetMapData(x, y) = s[x];
		}
	}

	//	プレイヤーの初期位置
	Vector3 startPoint = { 3, 1, 3 };

	//	床を生成
	auto floor = engine.Create<GameObject>("floor");
	floor->scale = {
		static_cast<float>(mapSizeX) * squareScale,
		1,
		static_cast<float>(mapSizeY) * squareScale
	};
	floor->position = { floor->scale.x, -1, floor->scale.z };
	floor->texColor = std::make_shared<Texture>("Res/floor.tga");
	floor->meshId = MeshId_box;

	auto texwall = std::make_shared<Texture>("Res/wall.tga");
	auto texCrystalBlue = std::make_shared<Texture>("Res/MeshData/crystal_blue.tga");

	for (int y = 0; y < mapSizeY; y++) {
		for (int x = 0; x < mapSizeX; x++) {
			const float posX = static_cast<float>(x + 0.5f) * squareSize;
			const float posZ = static_cast<float>(y + 0.5f) * squareSize;

			//	壁を生成
			if (GetMapData(x, y) == '#') {
				auto wall = engine.Create<GameObject>("wall", { posX, 0, posZ });
				wall->scale = { squareScale, squareScale, squareScale };
				wall->texColor = texwall;
				wall->meshId = MeshId_wall;
			}

			//	クリスタルを生成
			else if (GetMapData(x, y) == 'C') {
				auto crystal = engine.Create<GameObject>("crystal", { posX, 1, posZ });
				crystal->scale = { 0.5f, 0.5f, 0.5f };
				crystal->texColor = texCrystalBlue;
				crystal->meshId = MeshId_crystal;
			}

			//	プレーヤーの初期位置を設定
			else if (GetMapData(x, y) == 'S') {
				startPoint = { posX, 1, posZ };
			}

			//	敵を生成
			else if (GetMapData(x, y) == 'E') {
				auto enemy = engine.Create<GameObject>("enemy", { posX, 0, posZ });
				auto component = enemy->AddComponent<EnemySkull>();
				component->getMapData = [this](const Vector3& position) {
					int x, y;
					MapPosition(position, x, y);
					return GetMapData(x, y);
					};
			}

			//	ゴールオブジェクトを生成
			else if (GetMapData(x, y) == 'G') {
				auto goal = engine.Create<GameObject>("goal", { posX, 1, posZ });
				goal->AddComponent<GoalEvent>();
				auto collider = goal->AddComponent<AABBCollider>();
				collider->aabb = {
					{ -1.0f, -1.0f, -1.0f },
					{  1.0f,  1.0f,  1.0f },
				};
				collider->isTrigger = true;
			}
		}
	}

	//	カメラを操作するプレイヤーコンポーネントを生成
	auto player = engine.Create<GameObject>("player", { 0.0f, 10.0f, 0.0f });
	playerComponent = player->AddComponent<PlayerComponent>();
	auto collider = player->AddComponent<AABBCollider>();
	collider->aabb = {
		{ -0.5f, -1.0f, -0.5f },
		{  0.5f,  1.0f,  0.5f },
	};

	//	プレイヤーとカメラの初期位置を設定
	GameObject& camera = engine.GetMainCamera();
	player->position = camera.position = startPoint;
	player->rotation = camera.rotation = { 0.0f, 180.0f * Deg2Rad, 0.0f };

	//	OBJファイル表示テスト
	auto highpoly = engine.Create<GameObject>("skull_highpoly");
	highpoly->position = startPoint + Vector3{ 1.0f, 0.0f, 0.0f };
	highpoly->scale = { 2.0f, 2.0f, 2.0f };
	highpoly->rotation.y = -90.0f * Deg2Rad;
	highpoly->staticMesh = engine.GetStaticMesh(
		"Res/MeshData/skull/skull_highpoly.obj"
	);
	highpoly->texColor = std::make_shared<Texture>("Res/MeshData/skull.tga");

	//	比較用従来の表示テスト
	auto lowpoly = engine.Create<GameObject>("skull_lowpoly");
	lowpoly->position = startPoint;
	lowpoly->scale = { 2.0f, 2.0f, 2.0f };
	lowpoly->rotation.y = -90.0f * Deg2Rad;
	lowpoly->meshId = MeshId_skull;
	lowpoly->texColor = std::make_shared<Texture>("Res/MeshData/skull.tga");

	return true;	//	初期化成功
}

/*
 *	シーンを更新
 */
void MainGameScene::Update(Engine& engine, float deltaTime) {
	state(this, engine, deltaTime);
}

/*
 *	シーンの終了
 */
void MainGameScene::Finalize(Engine& engine) {
	engine.ClearGameObjects();
}

/*
 *	プレイ中の状態のシーンの更新
 */
void MainGameScene::StatePlaying(Engine& engine, float deltaTime) {
	constexpr Vector3 playerSize = { 1.0f, 1.5f, 1.0f };
	GameObject& camera = engine.GetMainCamera();
	camera.position = AdjustPosition(camera.position, playerSize);

	//	カメラのパラメータをプレイヤーオブジェクトにコピー
	auto player = playerComponent->GetOwner();
	player->position = camera.position;
	player->rotation = camera.rotation;

	//	てすと
	float fovY = engine.GetFovY();
	if (engine.GetKey(GLFW_KEY_1)) {
		fovY -= 30.0f * deltaTime;
		fovY = std::max(10.0f, fovY);
	}
	if (engine.GetKey(GLFW_KEY_2)) {
		fovY += 30.0f * deltaTime;
		fovY = std::min(120.0f, fovY);
	}
	engine.SetFovY(fovY);

	//	状態が死んでいるかどうか
	if (playerComponent->GetState() == PlayerComponent::State::Dead) {
		//	ゲームオーバーの画像を生成
		engine.CreateUIObject<UILayout>("Res/game_over.tga", { 0.0f, 0.0f }, 0.25f);
		//	タイトル画面に戻るボタンを生成
		auto button = engine.CreateUIObject<UIButton>("Res/return_button.tga", { 0.0f, -0.5f }, 0.1f);
		//	ボタンが押されたときの処理を追加
		button.second->onClick.push_back(
			[](UIButton* button) {
				Engine* engine = button->GetOwner()->GetEngine();
				engine->SetNextScene<TitleScene>();
			}
		);
		//	状態をゲームオーバーに変更
		state = &MainGameScene::StateGameOver;
	}
}

/*
 *	ゲームオーバー中の状態のシーンの更新
 */
void MainGameScene::StateGameOver(Engine& engine, float deltaTime) {

}

/*
 *	ワールド座標に対するマップ座標を取得する
 *	@param	position	ワールド座標
 *	@param	mapX		マップ座標X
 *	@param	mapY		マップ座標Y
 */
void MainGameScene::MapPosition(const Vector3& position, int& mapX, int& mapY) const {
	mapX = static_cast<int>(position.x / squareSize);
	mapY = static_cast<int>(position.z / squareSize);
}

/*
 *	壁にめり込まない座標を取得する
 *	@param	position	物体の座標
 *	@param	size		物体の大きさ
 *	@return				壁に埋まらない座標
 */
Vector3 MainGameScene::AdjustPosition(const Vector3& position, const Vector3& size) const {
	//	マップ座標を求める
	int mapX, mapY;
	MapPosition(position, mapX, mapY);

	//	隣接するマス目の位置
	constexpr struct {
		int x, y;
	} neighborOffsets[] = {
		{ +0, -1 }, { +0, +1 }, { -1, +0 }, { +1, +0 },	//	上下左右
		{ -1, -1 }, { +1, -1 }, { -1, +1 }, { +1, +1 },	//	斜め
	};

	//	周囲のマスを調べ、壁があったらAABBを作成する
	std::vector<AABB> walls;
	walls.reserve(std::size(neighborOffsets));
	for (const auto& ofs : neighborOffsets) {
		//	範囲外の場合は処理しない
		const int x = mapX + ofs.x;
		if (x < 0 || x >= mapSizeX)
			continue;

		const int y = mapY + ofs.y;
		if (y < 0 || y >= mapSizeY)
			continue;

		//	マス(x, y) に壁があるかどうか
		if (GetMapData(x, y) == '#') {
			//	AABBを作成
			AABB aabb;
			aabb.min = {
				static_cast<float>(x) * squareSize,
				-1,
				static_cast<float>(y) * squareSize
			};
			aabb.max = {
				aabb.min.x + squareSize, 1, aabb.min.z + squareSize
			};
			walls.push_back(aabb);
		}
	}

	//	物体のAABB
	AABB a;
	a.min = {
		position.x - size.x * 0.5f,
		position.y - size.y * 0.5f,
		position.z - size.z * 0.5f
	};
	a.max = {
		position.x + size.x * 0.5f,
		position.y + size.y * 0.5f,
		position.z + size.z * 0.5f
	};

	//	物体と壁の衝突判定
	Vector3 newPos = position;
	for (const auto& w : walls) {
		Vector3 penetration;
		if (Intersect(a, w, penetration)) {
			//	交差しているので座標を補正
			newPos.x -= penetration.x;
			newPos.y -= penetration.y;
			newPos.z -= penetration.z;

			a.min = {
				a.min.x - penetration.x,
				a.min.y - penetration.y,
				a.min.z - penetration.z
			};
			a.max = {
				a.max.x + penetration.x,
				a.max.y + penetration.y,
				a.max.z + penetration.z
			};
		}
	}

	return newPos;
}
