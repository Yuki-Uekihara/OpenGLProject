/*
 *  @file   TitleScene.cpp
 */

#include "TitleScene.h"
#include "MainGameScene.h"
#include "Engine/UILayout.h"
#include "Engine/UIButton.h"
#include "Engine/Engine.h"

 /*
  *  �^�C�g����ʂ̏���������
  */
bool TitleScene::Initialize(Engine& engine) {
	engine.CreateUIObject<UILayout>("Res/title_bg.tga", Vector2::zero, 1.4f);
	engine.CreateUIObject<UILayout>("Res/title_logo.tga", Vector2(0.0f, 0.6f), 0.3f);
	auto startButton = engine.CreateUIObject<UIButton>("Res/start_button.tga", Vector2(0.0f, -0.3f), 0.1f);


	//	�{�^���ɃC�x���g��ǉ��@�i�����_���Őݒ�j
	startButton.second->onClick.push_back(
		[this](UIButton* button) {
			fadeTimer = 1.0f;
			button->interactable = false;
		}
	);

	//	�t�F�[�h�pUI�I�u�W�F�N�g�̐���
	auto fade = engine.CreateUIObject<UILayout>("Res/white.tga", { 0.0f, 0.0f }, 1.0f);
	fadeObject = fade.first;
	//	��ʑS�̂Ƀt���X�g���b�`
	const Vector2 fbSize = engine.GetFramebufferSize();
	fadeObject->scale = { fbSize.x / fbSize.y, 1.0f, 1.0f };
	//	�J���[��ύX
	std::fill_n(fadeObject->color, 4, 0.0f);

	return true;
}

/*
 *	�^�C�g����ʂ̍X�V����
 */
void TitleScene::Update(Engine& engine, float deltaTime) {
	//	�^�C�}�[�ɍ��킹�ď��X�Ƀt�F�[�h�A�E�g������
	if (fadeTimer > 0) {
		fadeTimer -= deltaTime;
		fadeObject->color[3] = 1 - fadeTimer;		//	�����x

		//	�^�C�}�[��0�ȉ��ɂȂ�����V�[����؂�ւ���
		if (fadeTimer <= 0.0f) {
			engine.SetNextScene<MainGameScene>();
		}
	}
}


/*
 *  �^�C�g����ʂ̏I������
 */
void TitleScene::Finalize(Engine& engine) {
	engine.ClearGameObjects();
}