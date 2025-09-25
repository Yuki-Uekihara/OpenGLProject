/*
 *	@file	MainGameScene.h
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "Engine/Scene.h"
#include <vector>

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
