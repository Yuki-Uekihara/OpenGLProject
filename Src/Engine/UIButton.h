/*
 *	@file	UIButton.h
 */

#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#include "UILayout.h"
#include <functional>

/*
 *	UI用のボタン制御コンポーネント
 */
class UIButton : public UILayout {
private:
	bool prevButtonDown = false;		//	前回更新時のマウス左ボタンの状態

public:
	bool interactable = true;			//	反応するかどうか

public:
	Vector3 normalColor = { 1.0f, 1.0f, 1.0f };		//	通常色
	Vector3 highlightColor = { 1.3f, 1.3f, 1.3f };	//	選択色
	Vector3 pressedColor = { 0.7f, 0.7f, 0.7f };	//	押下色

	//	別名定義
	using ClickEvent = std::function<void(UIButton*)>;
	//	デリゲート
	std::vector<ClickEvent> onClick;

public:
	UIButton() = default;
	virtual ~UIButton() = default;

public:
	virtual void Update(float deltaTime) override;
};

#endif // !_UIBUTTON_H_
