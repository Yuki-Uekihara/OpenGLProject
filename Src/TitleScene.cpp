/*
 *  @file   TitleScene.cpp
 */

#include "TitleScene.h"
#include "MainGameScene.h"
#include "Engine/UILayout.h"
#include "Engine/UIButton.h"
#include "Engine/Engine.h"

 /*
  *  タイトル画面の初期化処理
  */
bool TitleScene::Initialize(Engine& engine) {
	engine.CreateUIObject<UILayout>("Res/title_bg.tga", Vector2::zero, 1.4f);
	engine.CreateUIObject<UILayout>("Res/title_logo.tga", Vector2(0.0f, 0.6f), 0.3f);
	auto startButton = engine.CreateUIObject<UIButton>("Res/start_button.tga", Vector2(0.0f, -0.3f), 0.1f);

	//	ボタンにイベントを追加　（ラムダ式で設定）
	startButton.second->onClick.push_back(
		[](UIButton* button) {
			Engine* engine = button->GetOwner()->GetEngine();
			engine->SetNextScene<MainGameScene>();
		}
	);

	return true;
}

/*
 *  タイトル画面の終了処理
 */
void TitleScene::Finalize(Engine& engine) {
	engine.ClearGameObjects();
}
