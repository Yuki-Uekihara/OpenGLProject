/*
 *	@file	UIButton.h
 */

#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#include "UILayout.h"
#include <functional>

/*
 *	UI�p�̃{�^������R���|�[�l���g
 */
class UIButton : public UILayout {
private:
	bool prevButtonDown = false;		//	�O��X�V���̃}�E�X���{�^���̏��

public:
	bool interactable = true;			//	�������邩�ǂ���

public:
	Vector3 normalColor = { 1.0f, 1.0f, 1.0f };		//	�ʏ�F
	Vector3 highlightColor = { 1.3f, 1.3f, 1.3f };	//	�I��F
	Vector3 pressedColor = { 0.7f, 0.7f, 0.7f };	//	�����F

	//	�ʖ���`
	using ClickEvent = std::function<void(UIButton*)>;
	//	�f���Q�[�g
	std::vector<ClickEvent> onClick;

public:
	UIButton() = default;
	virtual ~UIButton() = default;

public:
	virtual void Update(float deltaTime) override;
};

#endif // !_UIBUTTON_H_
