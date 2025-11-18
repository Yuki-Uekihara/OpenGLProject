/*
 *  @file   TitleScene.cpp
 */

#include "TitleScene.h"
#include "MainGameScene.h"
#include "Engine/UILayout.h"
#include "Engine/UIButton.h"
#include "Engine/Engine.h"
#include "Engine/EasyAudio/EasyAudio.h"
#include "AudioSettings.h"

 /*
  *  タイトル画面の初期化処理
  */
bool TitleScene::Initialize(Engine& engine) {
	engine.CreateUIObject<UILayout>("Res/title_bg.tga", Vector2::zero, 1.4f);
	engine.CreateUIObject<UILayout>("Res/title_logo.tga", Vector2(0.0f, 0.6f), 0.3f);
	auto startButton = engine.CreateUIObject<UIButton>("Res/start_button.tga", Vector2(0.0f, -0.3f), 0.1f);


	//	ボタンにイベントを追加　（ラムダ式で設定）
	startButton.second->onClick.push_back(
		[this](UIButton* button) {
			fadeTimer = 1.0f;
			button->interactable = false;
			EasyAudio::PlayOneShot(SE::buttonClick);
		}
	);

	//	フェード用UIオブジェクトの生成
	auto fade = engine.CreateUIObject<UILayout>("Res/white.tga", { 0.0f, 0.0f }, 1.0f);
	fadeObject = fade.first;
	//	画面全体にフルストレッチ
	const Vector2 fbSize = engine.GetFramebufferSize();
	fadeObject->scale = { fbSize.x / fbSize.y, 1.0f, 1.0f };
	//	カラーを変更
	std::fill_n(fadeObject->color, 4, 0.0f);

	//	BGMを再生
	EasyAudio::Play(AudioPlayer::bgm, BGM::title, 1.0f, true);

	return true;
}

/*
 *	タイトル画面の更新処理
 */
void TitleScene::Update(Engine& engine, float deltaTime) {
	//	タイマーに合わせて徐々にフェードアウトさせる
	if (fadeTimer > 0) {
		fadeTimer -= deltaTime;
		fadeObject->color[3] = 1 - fadeTimer;		//	透明度

		//	タイマーが0以下になったらシーンを切り替える
		if (fadeTimer <= 0.0f) {
			engine.SetNextScene<MainGameScene>();
		}
	}
}


/*
 *  タイトル画面の終了処理
 */
void TitleScene::Finalize(Engine& engine) {
	engine.ClearGameObjects();
	EasyAudio::Stop(AudioPlayer::bgm);
}