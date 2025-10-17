/*
 *	@file	EnemySkull.cpp
 */

#include "EnemySkull.h"
#include "Engine/AABBCollider.h"
#include "Engine/Engine.h"

/*
 *	�R���|�[�l���g�̏�����
 */
void EnemySkull::Awake() {
	//	���f���̐ݒ�
	auto owner = GetOwner();
	owner->meshId = MeshId_skull;
	owner->texColor = std::make_shared<Texture>("Res/MeshData/skull.tga");

	//	�R���C�_�[�̐ݒ�
	auto collider = owner->AddComponent<AABBCollider>();
	collider->aabb = {
		{ -0.25f, -0.25f, -0.25f },
		{  0.25f,  0.25f,  0.25f }
	};
	collider->isTrigger = true;
}

/*
 *	�R���|�[�l���g�̍X�V
 */
void EnemySkull::Update(float deltaTime) {
	auto owner = GetOwner();

	//	���쐧��p�^�C�}�[�̍X�V
	timer += deltaTime;

	//	�㉺�ɗh�炷
	constexpr float baseY = 1.0f;		//	��̍���(m)
	constexpr float range = 0.1f;		//	�h�炷����(m)
	constexpr float cycle = 5.0f;		//	�h�炷���o(�b)
	owner->position.y = baseY + std::sinf(timer * cycle) * range;

	if (getMapData) {
		const Vector3 front = { std::cosf(owner->rotation.y), 0.0f, std::sinf(owner->rotation.y) };
		const int data = getMapData(owner->position + front);

		//	�O�����ǂ������͕����̋�؂肩�ǂ���
		if (data != '#' && data != '|' && data != '-') {
			//	�O�i
			const float speed = 1.0f;
			owner->position.x += front.x * speed * deltaTime;
			owner->position.z += front.z * speed * deltaTime;
		}
		else {
			//	���ɐ���
			owner->rotation.y += 90.0f * Deg2Rad;
		}
	}
}
