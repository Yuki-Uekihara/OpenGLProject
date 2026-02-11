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
	const size_t tgaHeaderSize = 18;						//	ヘッダー情報のバイト数
	const int imageOffset = tgaHeaderSize + buffer[0];		//	画像データの位置
	const uint8_t imageType = buffer[2];					//	画像形式
	const int pixelBytes = (buffer[16] + 7) / 8;			//	1ピクセルのバイト数
	width = buffer[12] + buffer[13] * 256;
	height = buffer[14] + buffer[15] * 256;

	/*	
	 *	圧縮形式の場合は展開する
	 *	 0(0b0000):画像なし
	 *	 1(0b0001):インデックス（無圧縮）
	 *	 2(0b0010):トゥルーカラー（無圧縮）
	 *	 3(0b0011):白黒（無圧縮）
	 *	 9(0b1001):インデックス（RLE）
	 *	10(0b1010):トゥルーカラー（RLE）
	 *	11(0b1011):白黒（RLE）
	 */
	if (imageType & 0b1000) {
		//	展開用のバッファを用意
		const int imageBytes = width * height * pixelBytes;
		std::vector<uint8_t> tmp(imageOffset + imageBytes);

		//	swapに備えて画像データよりも前の部分をコピー
		std::copy_n(buffer.begin(), imageOffset, tmp.begin());

		//	画像データを展開
		const uint8_t* src = buffer.data() + imageOffset;	//	圧縮データのアドレス
		uint8_t* dest = tmp.data() + imageOffset;			//	展開先のアドレス
		const uint8_t* const destEnd = dest + imageBytes;	//	展開終了アドレス

		while (dest != destEnd) {
			//	ヘッダからIDとデータ数を取得
			const int isRLE = *src & 0x80;
			const int count = (*src & 0x7f) + 1;
			src++;	//	アドレスを次に進める

			if (isRLE) {
				//	圧縮データの場合は指定回数コピーする
				for (int i = 0; i < count; i++) {
					//memcpy_s(dest, destEnd - dest, src, pixelBytes);
					memcpy(dest, src, pixelBytes);
					dest += pixelBytes;
				}
				src += pixelBytes;
			}
			else{
				//	無圧縮データの場合は全体をコピーする
				const int dataBytes = pixelBytes * count;
				//memcpy_s(dest, destEnd - dest, src, dataBytes);
				memcpy(dest, src, dataBytes);
				dest += dataBytes;
				src += dataBytes;
			}
		}

		//	swap処理
		buffer.swap(tmp);
	}


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

	//	1ピクセルのドット数に対応する形式のセットの検索用
	struct Format {
		int pixelDepth;			//	1ピクセルのドット数
		GLenum imageFormat;		//	データ形式
		GLenum imageType;		//	データの型
		GLenum gpuFormat;		//	GPU側のデータ形式
	};
	constexpr Format formatList[] = {
		{ 32, GL_BGRA, GL_UNSIGNED_BYTE, GL_RGBA8 },
		{ 24, GL_BGR,  GL_UNSIGNED_BYTE, GL_RGB8  },
		{ 16, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGB5_A1  },
		{ 15, GL_BGR, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGB5  },
		{  8, GL_RED, GL_UNSIGNED_BYTE, GL_R8  },
	};

	const Format* format = std::find_if(
		formatList,
		std::end(formatList) - 1,
		[pixelDepth = buffer[16]](const  Format& f) { return f.pixelDepth == pixelDepth; }
	);

	//	現在のアラインメントを記録
	GLint alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

	//	画像のアラインメントを決定
	//	一行のバイト数が4で割り切れるときは4, 2で割り切れるときは2, それ以外は1
	constexpr int alignmentList[] = { 4, 1, 2, 1 };
	const int lineByteSize = width * pixelBytes;		//	1行のバイト数
	const int imageAlignment = alignmentList[lineByteSize % 4];

	//	アラインメントを変更
	if (alignment != imageAlignment) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, imageAlignment);
	}

	//	テクスチャを作成
	GLuint object = 0;		//	管理番号
	glCreateTextures(GL_TEXTURE_2D, 1, &object);
	glTextureStorage2D(object, 1, format->gpuFormat, width, height);
	glTextureSubImage2D(
		object,
		0,
		0, 0,
		width, height,
		format->imageFormat,
		format->imageType,
		buffer.data() + tgaHeaderSize
	);
	id = object;
	name = filename;

	//	アラインメントを元に戻す
	if (alignment != imageAlignment) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	}

	//	グレースケール（白黒）のテクスチャ用
	if (format->imageFormat == GL_RED) {
		glTextureParameteri(object, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTextureParameteri(object, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTextureParameteri(object, GL_TEXTURE_SWIZZLE_B, GL_RED);
	}
}

/*
 *	デストラクタ
 */
Texture::~Texture() {
	glDeleteTextures(1, &id);
}

