/*
 *	@file	MainGameScene.h
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "Engine/Scene.h"
#include "Engine/VecMath.h"
#include <vector>
#include <functional>

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

	std::shared_ptr<class PlayerComponent> playerComponent;	//	プレイヤー

	//	別名定義
	using State = std::function<void(MainGameScene*, Engine&, float)>;
	State state = &MainGameScene::StatePlaying;

public:
	MainGameScene() = default;
	virtual ~MainGameScene() = default;

public:
	virtual bool Initialize(Engine& engine) override;
	virtual void Update(Engine& engine, float deltaTime) override;
	virtual void Finalize(Engine& engine) override;

public:
	//	状態に対応する更新
	void StatePlaying(Engine& engine, float deltaTime);
	void StateGameOver(Engine& engine, float deltaTime);

private:
	inline char GetMapData(int x, int y) const {
		return map[y * mapSizeX + x];
	}

	inline char& GetMapData(int x, int y) {
		return map[y * mapSizeX + x];
	}

	void MapPosition(const Vector3& position, int& mapX, int& mapY) const;

	Vector3 AdjustPosition(const Vector3& position, const Vector3& size) const;
};

#endif // !_MAINGAMESCENE_H_
