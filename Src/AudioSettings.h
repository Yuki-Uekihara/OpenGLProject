/*
 *	@file	AudioSettings.h
 */

#ifndef _AUDIOSETTINGS_H_
#define _AUDIOSETTINGS_H_

/*
 *	音声再生プレイヤー番号
 */
namespace AudioPlayer {
	inline constexpr int bgm = 0;	//	BGMの再生に使う管理番号
}

/*
 *	BGM設定
 */
namespace BGM {
	inline constexpr char title[] = "Res/Audio/____";	//	タイトルのBGM
	inline constexpr char mainGame[] = "Res/Audio/____";	//	ゲーム中のBGM
}

/*
 *	SE設定
 */
namespace SE {
	inline constexpr char buttonClick[] = "Res/Audio/____";	//	ボタン押下音
	inline constexpr char doorOpen[] = "Res/Audio/____";	//	ドア開閉音
	inline constexpr char leverMove[] = "Res/Audio/____";	//	レバー動作音
	inline constexpr char goalEvent[] = "Res/Audio/____";	//	ゴール到達音
	inline constexpr char gameOverEvent[] = "Res/Audio/____";	//	敵接触音
}

#endif // !_AUDIOSETTINGS_H_
