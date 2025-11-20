/*
 *	@file	GoalEvent.cpp
 */

#include "GoalEvent.h"
#include "Engine/Engine.h"
#include "Engine/UILayout.h"
#include "Engine/UIButton.h"
#include "TitleScene.h"
#include "Engine/EasyAudio/EasyAudio.h"
#include "AudioSettings.h"

 /*
  *	タイトル画面の更新処理
  */
void GoalEvent::Update(float deltaTime) {
	Engine* engine = GetOwner()->GetEngine();

	//	タイマーに合わせて徐々にフェードアウトさせる
	if (fadeTimer > 0) {
		fadeTimer -= deltaTime;
		fadeObject->color[3] = 1 - fadeTimer;		//	透明度

		//	タイマーが0以下になったらシーンを切り替える
		if (fadeTimer <= 0.0f) {
			engine->SetNextScene<TitleScene>();
		}
	}
}


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
	engine->CreateUIObject<UILayout>("Res/goal_text.tga", { 0.0f, 0.0f }, 0.1f);

	//	ボタンオブジェクトを生成
	auto button = engine->CreateUIObject<UIButton>("Res/return_button.tga", { 0.0f, -0.5f }, 0.1f);
	//	ボタンのクリック機能の追加
	button.second->onClick.push_back(
		[this](UIButton* button) {
			button->interactable = false;
			fadeTimer = 1.0f;
			EasyAudio::PlayOneShot(SE::buttonClick);
		}
	);

	//	フェード用UIオブジェクトの生成
	auto fade = engine->CreateUIObject<UILayout>("Res/white.tga", { 0.0f, 0.0f }, 1.0f);
	fadeObject = fade.first;
	//	画面全体にフルストレッチ
	const Vector2 fbSize = engine->GetFramebufferSize();
	fadeObject->scale = { fbSize.x / fbSize.y, 1.0f, 1.0f };
	//	カラーを変更
	std::fill_n(fadeObject->color, 4, 0.0f);

	EasyAudio::PlayOneShot(SE::goalEvent);
}
