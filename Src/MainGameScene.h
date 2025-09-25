/*
 *	@file	MainGameScene.h
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "Engine/Scene.h"
#include <vector>

/*
 *	メインゲームシーン
 */
class MainGameScene : public Scene {
private:
	std::vector<char> map;					//	マップデータ
	int mapSizeX = 11;						//	広さ(横)
	int mapSizeY = 11;						//	広さ(縦)
	float squareSize = 2;					//	1マスの大きさ
	float squareScale = squareSize / 2;		//	1マスの拡大率

public:
	MainGameScene() = default;
	virtual ~MainGameScene() = default;

public:
	virtual bool Initialize(Engine& engine) override;

private:
	inline char GetMapData(int x, int y) const {
		return map[y * mapSizeX + x];
	}

	inline char& GetMapData(int x, int y) {
		return map[y * mapSizeX + x];
	}
};

#endif // !_MAINGAMESCENE_H_
