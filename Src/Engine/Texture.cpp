/*
 *	@file	Texture.cpp
 */

#include "Texture.h"
#include <vector>
#include <fstream>
#include <filesystem>
#include <Windows.h>

 /*
  *	コンストラクタ
  */
Texture::Texture(const char* filename) {
	//	ファイルを開く
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file) {
		return;	//	失敗
	}

	//	ファイルのサイズを取得
	const size_t fileSize = std::filesystem::file_size(filename);
	//	ファイルのサイズ分の領域を確保
	std::vector<uint8_t> buffer(fileSize);
	//	読み込み + コピー
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

	//	ファイルを閉じる
	file.close();

	//	TGAヘッダから情報を取得
	const size_t tgaHeaderSize = 18;	//	ヘッダー情報のバイト数
	width = buffer[12] + buffer[13] * 256;
	height = buffer[14] + buffer[15] * 256;

	//	テクスチャを作成
	GLuint object = 0;		//	管理番号
	glCreateTextures(GL_TEXTURE_2D, 1, &object);
	glTextureStorage2D(object, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(
		object,
		0,
		0, 0,
		width, height,
		GL_BGRA,
		GL_UNSIGNED_BYTE,
		buffer.data() + tgaHeaderSize
	);

	id = object;
	name = filename;
}

/*
 *	デストラクタ
 */
Texture::~Texture() {
	glDeleteTextures(1, &id);
}

