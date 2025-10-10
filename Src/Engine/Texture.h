/*
 *	@file	Texture.h
 */

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "glad/glad.h"
#include <string>
#include <memory>

/*
 *	テクスチャ管理クラス
 */
class Texture {
private:
	std::string name;			//	テクスチャ名(デバッグ用)
	GLuint id = 0;				//	管理番号
	int width = 0, height = 0;	//	幅と高さ

public:
	explicit Texture(const char* filename);
	~Texture();
	//	コピーと代入の禁止
	Texture(const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;

public:
	//	名前の取得
	inline const std::string& GetName() const { return name; }

	//	管理番号の取得
	operator GLuint() const { return id; }

	//	幅の取得
	inline int GetWidth() const { return width; }

	//	高さの取得
	inline int GetHeight() const { return height; }

	//	テクスチャのアスペクト比を取得
	inline float GetAspectRatio() const {
		return static_cast<float>(width) / static_cast<float>(height);
	}
};
//	別名定義
using TexturePtr = std::shared_ptr<Texture>;

#endif // !_TEXTURE_H_
