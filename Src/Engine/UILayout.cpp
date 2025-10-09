/*
 *	@file	UILayout.cpp
 */

#include "UILayout.h"
#include "Engine.h"

/*
 *	UIレイアウトの実行を開始 
 */
void UILayout::Start() {
	//	最初に指定された座標を「カメラから見た位置」とする
	basePosition = GetOwner()->position;
}

/*
 *	更新イベント
 *	@param deltaTime
 */
void UILayout::Update(float deltaTime) {
	//	常にカメラの方を向く
	GameObject* owner = GetOwner();
	const Engine* engine = owner->GetEngine();
	const GameObject& camera = engine->GetMainCamera();
	owner->rotation.y = camera.rotation.y;

	//	常にカメラの正面に位置するように座標を調整
	//	回転の公式によってカメラとの早退座標を求める
	Vector3 pos = basePosition;		//	角度0度の場合

	//	y座標が カメラの映る範囲の上端と下端になるようにZ値を調整
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
