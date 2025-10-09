/*
 *	@file	GoalEvent.cpp
 */

#include "GoalEvent.h"
#include "Engine/Engine.h"
#include "Engine/UILayout.h"

/*
 *	ゴール演出の衝突イベント
 *	@param self
 *	@param other
 */
void GoalEvent::OnCollision(const ComponentPtr& self, const ComponentPtr& other) {
	//	既にプレイヤーと衝突していたら処理しない
	if (triggerd || other->GetOwner()->name != "player")
		return;

	triggerd = true;	//	衝突済みにする

	//	メッセージオブジェクトを生成
	Engine* engine = GetOwner()->GetEngine();
	auto obj = engine->Create<GameObject>("goal text", Vector3::back);
	
	obj->texColor = std::make_shared<Texture>("Res/goal_text.tga");
	obj->meshId = 2;
	obj->renderQueue = RenderQueue_overlay;
	//	コンポーネントの追加
	obj->AddComponent<UILayout>();
}
