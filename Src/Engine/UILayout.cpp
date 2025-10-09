/*
 *	@file	UILayout.cpp
 */

#include "UILayout.h"
#include "Engine.h"

/*
 *	UI���C�A�E�g�̎��s���J�n 
 */
void UILayout::Start() {
	//	�ŏ��Ɏw�肳�ꂽ���W���u�J�������猩���ʒu�v�Ƃ���
	basePosition = GetOwner()->position;
}

/*
 *	�X�V�C�x���g
 *	@param deltaTime
 */
void UILayout::Update(float deltaTime) {
	//	��ɃJ�����̕�������
	GameObject* owner = GetOwner();
	const Engine* engine = owner->GetEngine();
	const GameObject& camera = engine->GetMainCamera();
	owner->rotation.y = camera.rotation.y;

	//	��ɃJ�����̐��ʂɈʒu����悤�ɍ��W�𒲐�
	//	��]�̌����ɂ���ăJ�����Ƃ̑��ލ��W�����߂�
	Vector3 pos = basePosition;		//	�p�x0�x�̏ꍇ

	//	y���W�� �J�����̉f��͈͂̏�[�Ɖ��[�ɂȂ�悤��Z�l�𒲐�
	pos.z = -engine->GetFovScale();

	const float s = std::sinf(camera.rotation.y);
	const float c = std::cosf(camera.rotation.y);
	pos = {
		pos.x * c + pos.z * s,
		pos.y,
		pos.x * -s + pos.z * c
	};
	owner->position = camera.position + pos;
}
