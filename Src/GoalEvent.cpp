/*
 *	@file	GoalEvent.cpp
 */

#include "GoalEvent.h"
#include "Engine/Engine.h"
#include "Engine/UILayout.h"
#include "Engine/UIButton.h"
#include "TitleScene.h"

 /*
  *	�^�C�g����ʂ̍X�V����
  */
void GoalEvent::Update(float deltaTime) {
	Engine* engine = GetOwner()->GetEngine();

	//	�^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
	if (fadeTimer > 0) {
		fadeTimer -= deltaTime;
		fadeObject->color[3] = 1 - fadeTimer;		//	�����x

		//	�^�C�}�[��0�ȉ��ɂȂ�����V�[����؂�ւ���
		if (fadeTimer <= 0.0f) {
			engine->SetNextScene<TitleScene>();
		}
	}
}


/*
 *	�S�[�����o�̏Փ˃C�x���g
 *	@param self
 *	@param other
 */
void GoalEvent::OnCollision(const ComponentPtr& self, const ComponentPtr& other) {
	//	���Ƀv���C���[�ƏՓ˂��Ă����珈�����Ȃ�
	if (triggerd || other->GetOwner()->name != "player")
		return;

	triggerd = true;	//	�Փˍς݂ɂ���

	//	���b�Z�[�W�I�u�W�F�N�g�𐶐�
	Engine* engine = GetOwner()->GetEngine();
	engine->CreateUIObject<UILayout>("Res/goal_text.tga", { 0.0f, 0.0f }, 0.1f);

	//	�{�^���I�u�W�F�N�g�𐶐�
	auto button = engine->CreateUIObject<UIButton>("Res/return_button.tga", { 0.0f, -0.5f }, 0.1f);
	//	�{�^���̃N���b�N�@�\�̒ǉ�
	button.second->onClick.push_back(
		[this](UIButton* button) {
			button->interactable = false;
			fadeTimer = 1.0f;
		}
	);

	//	�t�F�[�h�pUI�I�u�W�F�N�g�̐���
	auto fade = engine->CreateUIObject<UILayout>("Res/white.tga", { 0.0f, 0.0f }, 1.0f);
	fadeObject = fade.first;
	//	��ʑS�̂Ƀt���X�g���b�`
	const Vector2 fbSize = engine->GetFramebufferSize();
	fadeObject->scale = { fbSize.x / fbSize.y, 1.0f, 1.0f };
	//	�J���[��ύX
	std::fill_n(fadeObject->color, 4, 0.0f);
}
