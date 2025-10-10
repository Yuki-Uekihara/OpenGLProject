/*
 * @file	UIButton.cpp
 */

#include "UIButton.h"
#include "Engine.h"

/*
 *	更新イベント
 */
void UIButton::Update(float deltaTime) {
	//	基底クラスのメンバ関数を実行
	UILayout::Update(deltaTime);

	GameObject* owner = GetOwner();
	Engine* engine = owner->GetEngine();

	const Vector2 mouse = engine->GetMousePosition();
	if (std::fabsf(mouse.x - basePosition.x) < owner->scale.x &&
		std::fabsf(mouse.y - basePosition.y) < owner->scale.y) {
		//	カーソルがボタンの領域内にある
		const bool buttonDown = engine->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		//	左ボタンが押されている
		if (buttonDown) {
			std::copy_n(&pressedColor.x, 3, owner->color);
		}
		//	左ボタンが離されている
		else {
			std::copy_n(&highlightColor.x, 3, owner->color);

			//	前回のフレームで押していたら
			if (prevButtonDown) {
				//	ボタンに登録されているデリゲートの処理
				for (auto& event : onClick) {
					event(this);
				}
			}
		}

		//	マウスボタンの更新
		prevButtonDown = buttonDown;
	}
	else {
		//	カーソルがボタンの領域外にある
		std::copy_n(&normalColor.x, 3, owner->color);

		//	マウスボタンの更新
		prevButtonDown = false;
	}
}
