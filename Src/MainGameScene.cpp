/*
 *	@file	MainGameScene.cpp
 */

#include "MainGameScene.h"
#include "Engine/Engine.h"
#include "Engine/Collision.h"
#include "PlayerComponent.h"
#include "GoalEvent.h"

#include <fstream>
#include <string>



/*
 *	�V�[��������������
 *	@return	true	����
 */
bool MainGameScene::Initialize(Engine& engine) {
	//	�t�@�C������f�[�^��ǂݍ���
	std::ifstream file("Res/maze00.txt");
	if (!file)
		return false;	//	���s

	file >> mapSizeX >> mapSizeY;	//	�}�b�v�̑傫����ǂݍ���
	file.ignore();					//	���s���΂�

	//	�f�[�^��ǂݍ���
	map.resize(mapSizeX * mapSizeY);
	for (int y = 0; y < mapSizeY; y++) {
		std::string s;
		std::getline(file, s);

		for (int x = 0; x < mapSizeX; x++) {
			GetMapData(x, y) = s[x];
		}
	}

	//	���𐶐�
	auto floor = engine.Create<GameObject>("floor");
	floor->scale = {
		static_cast<float>(mapSizeX) * squareScale,
		1,
		static_cast<float>(mapSizeY) * squareScale
	};
	floor->position = { floor->scale.x, -1, floor->scale.z };
	floor->texColor = std::make_shared<Texture>("Res/floor.tga");
	floor->meshId = 0;

	auto texwall = std::make_shared<Texture>("Res/wall.tga");
	auto texCrystalBlue = std::make_shared<Texture>("Res/MeshData/crystal_blue.tga");

	for (int y = 0; y < mapSizeY; y++) {
		for (int x = 0; x < mapSizeX; x++) {
			const float posX = static_cast<float>(x + 0.5f) * squareSize;
			const float posZ = static_cast<float>(y + 0.5f) * squareSize;
			
			//	�ǂ𐶐�
			if (GetMapData(x, y) == '#') {
				auto wall = engine.Create<GameObject>("wall", { posX, 0.5f * squareSize, posZ });
				wall->scale = { squareScale, squareScale, squareScale };
				wall->texColor = texwall;
				wall->meshId = 0;
			}
			
			//	�N���X�^���𐶐�
			else if (GetMapData(x, y) == 'C') {
				auto crystal = engine.Create<GameObject>("crystal", { posX, 1, posZ });
				crystal->scale = { 0.5f, 0.5f, 0.5f };
				crystal->texColor = texCrystalBlue;
				crystal->meshId = 1;
			}

			//	�S�[���I�u�W�F�N�g�𐶐�
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

	//	�J�����𑀍삷��v���C���[�R���|�[�l���g�𐶐�
	auto player = engine.Create<GameObject>("player", { 0.0f, 10.0f, 0.0f });
	playerComponent = player->AddComponent<PlayerComponent>();
	auto collider = player->AddComponent<AABBCollider>();
	collider->aabb = {
		{ -0.5f, -1.0f, -0.5f },
		{  0.5f,  1.0f,  0.5f },
	};

	return true;	//	����������
}

/*
 *	�V�[�����X�V
 */
void MainGameScene::Update(Engine& engine, float deltaTime) {
	constexpr Vector3 playerSize = { 1.0f, 1.5f, 1.0f };
	GameObject& camera = engine.GetMainCamera();
	camera.position = AdjustPosition(camera.position, playerSize);

	//	�J�����̃p�����[�^���v���C���[�I�u�W�F�N�g�ɃR�s�[
	auto player = playerComponent->GetOwner();
	player->position = camera.position;
	player->rotation = camera.rotation;
}

/*
 *	���[���h���W�ɑ΂���}�b�v���W���擾����
 *	@param	position	���[���h���W
 *	@param	mapX		�}�b�v���WX
 *	@param	mapY		�}�b�v���WY
 */
void MainGameScene::MapPosition(const Vector3& position, int& mapX, int& mapY) const {
	mapX = static_cast<int>(position.x / squareSize);
	mapY = static_cast<int>(position.z / squareSize);
}

/*
 *	�ǂɂ߂荞�܂Ȃ����W���擾����
 *	@param	position	���̂̍��W
 *	@param	size		���̂̑傫��
 *	@return				�ǂɖ��܂�Ȃ����W
 */
Vector3 MainGameScene::AdjustPosition(const Vector3& position, const Vector3& size) const {
	//	�}�b�v���W�����߂�
	int mapX, mapY;
	MapPosition(position, mapX, mapY);
	
	//	�אڂ���}�X�ڂ̈ʒu
	constexpr struct {
		int x, y;
	} neighborOffsets[] = {
		{ +0, -1 }, { +0, +1 }, { -1, +0 }, { +1, +0 },	//	�㉺���E
		{ -1, -1 }, { +1, -1 }, { -1, +1 }, { +1, +1 },	//	�΂�
	};

	//	���͂̃}�X�𒲂ׁA�ǂ���������AABB���쐬����
	std::vector<AABB> walls;
	walls.reserve(std::size(neighborOffsets));
	for (const auto& ofs : neighborOffsets) {
		//	�͈͊O�̏ꍇ�͏������Ȃ�
		const int x = mapX + ofs.x;
		if (x < 0 || x >= mapSizeX)
			continue;

		const int y = mapY + ofs.y;
		if (y < 0 || y >= mapSizeY)
			continue;

		//	�}�X(x, y) �ɕǂ����邩�ǂ���
		if (GetMapData(x, y) == '#') {
			//	AABB���쐬
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

	//	���̂�AABB
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

	//	���̂ƕǂ̏Փ˔���
	Vector3 newPos = position;
	for (const auto& w : walls) {
		Vector3 penetration;
		if (Intersect(a, w, penetration)) {
			//	�������Ă���̂ō��W��␳
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
