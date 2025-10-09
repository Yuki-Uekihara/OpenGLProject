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

	//	格納方向が「上から下」の場合、データを上下反転
	const bool topToBottom = buffer[17] & 0b0010'0000;
	if (topToBottom) {
		const int pixcelDepth = buffer[16];					//	1ピクセルのビット数
		const int lineByteSize = width * pixcelDepth / 8;	//	1行のバイト数
		uint8_t* top = buffer.data() + tgaHeaderSize;		//	上の行の位置
		uint8_t* bottom = top + lineByteSize * (height - 1);//	下の行の位置
		std::vector<uint8_t> tmp(lineByteSize);				//	入れ替え用バッファ

		//	上下の行が入れ替わるまで繰り返す
		while (top < bottom) {
			std::copy_n(top, lineByteSize, tmp.data());		//	「上の行のコピー」を作る
			std::copy_n(bottom, lineByteSize, top);			//	下の行を上の行に上書き
			std::copy_n(tmp.data(), lineByteSize, bottom);	//	「上の行のコピー」を下の行に上書き
			top += lineByteSize;							//	上の行を1行下に移動
			bottom -= lineByteSize;							//	下の行を1行上に移動
		}
	}


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

