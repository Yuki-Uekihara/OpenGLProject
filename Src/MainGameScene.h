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
 *	���C���Q�[���V�[��
 */
class MainGameScene : public Scene {
private:
	std::vector<char> map;					//	�}�b�v�f�[�^
	int mapSizeX = 11;						//	�L��(��)
	int mapSizeY = 11;						//	�L��(�c)
	float squareSize = 2;					//	1�}�X�̑傫��
	float squareScale = squareSize / 2;		//	1�}�X�̊g�嗦

	std::shared_ptr<class PlayerComponent> playerComponent;	//	�v���C���[

	//	�ʖ���`
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
	//	��ԂɑΉ�����X�V
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
