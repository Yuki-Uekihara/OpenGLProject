/*
 *	@file	MainGameScene.cpp
 */

#include "MainGameScene.h"
#include "Engine/Engine.h"
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

	//	床を生成
	auto floor = engine.Create<GameObject>("floor");
	floor->scale = {
		static_cast<float>(mapSizeX) * squareScale,
		1,
		static_cast<float>(mapSizeY) * squareScale
	};
	floor->position = { floor->scale.x, -1, floor->scale.z };
	floor->texColor = std::make_shared<Texture>("Res/floor.tga");

	//	壁を生成
	auto texwall = std::make_shared<Texture>("Res/wall.tga");
	for (int y = 0; y < mapSizeY; y++) {
		for (int x = 0; x < mapSizeX; x++) {
			const float posX = static_cast<float>(x + 0.5f) * squareSize;
			const float posZ = static_cast<float>(y + 0.5f) * squareSize;
			if (GetMapData(x, y) == '#') {
				auto wall = engine.Create<GameObject>("wall", { posX, 0.5f * squareSize, posZ });
				wall->scale = { squareScale, squareScale, squareScale };
				wall->texColor = texwall;
			}
		}
	}



	return true;	//	初期化成功
}