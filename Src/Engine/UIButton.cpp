/*
 * @file	UIButton.cpp
 */

#include "UIButton.h"
#include "Engine.h"

/*
 *	�X�V�C�x���g
 */
void UIButton::Update(float deltaTime) {
	//	���N���X�̃����o�֐������s
	UILayout::Update(deltaTime);

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	const Vector2 mouse = engine->GetMousePosition();
	if (std::fabsf(mouse.x - basePosition.x) < owner->scale.x &&
		std::fabsf(mouse.y - basePosition.y) < owner->scale.y) {
		//	�J�[�\�����{�^���̗̈���ɂ���
		const bool buttonDown = engine->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		//	���{�^����������Ă���
		if (buttonDown) {
			std::copy_n(&pressedColor.x, 3, owner->color);
		}
		//	���{�^����������Ă���
		else {
			std::copy_n(&highlightColor.x, 3, owner->color);

			//	�O��̃t���[���ŉ����Ă�����
			if (prevButtonDown) {
				//	�{�^���ɓo�^����Ă���f���Q�[�g�̏���
				for (auto& event : onClick) {
					event(this);
				}
			}
		}

		//	�}�E�X�{�^���̍X�V
		prevButtonDown = buttonDown;
	}
	else {
		//	�J�[�\�����{�^���̗̈�O�ɂ���
		std::copy_n(&normalColor.x, 3, owner->color);

		//	�}�E�X�{�^���̍X�V
		prevButtonDown = false;
	}
}
