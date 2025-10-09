/*
 *	@file	Texture.cpp
 */

#include "Texture.h"
#include <vector>
#include <fstream>
#include <filesystem>
#include <Windows.h>

 /*
  *	�R���X�g���N�^
  */
Texture::Texture(const char* filename) {
	//	�t�@�C�����J��
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file) {
		return;	//	���s
	}

	//	�t�@�C���̃T�C�Y���擾
	const size_t fileSize = std::filesystem::file_size(filename);
	//	�t�@�C���̃T�C�Y���̗̈���m��
	std::vector<uint8_t> buffer(fileSize);
	//	�ǂݍ��� + �R�s�[
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

	//	�t�@�C�������
	file.close();

	//	TGA�w�b�_��������擾
	const size_t tgaHeaderSize = 18;	//	�w�b�_�[���̃o�C�g��
	width = buffer[12] + buffer[13] * 256;
	height = buffer[14] + buffer[15] * 256;

	//	�i�[�������u�ォ�牺�v�̏ꍇ�A�f�[�^���㉺���]
	const bool topToBottom = buffer[17] & 0b0010'0000;
	if (topToBottom) {
		const int pixcelDepth = buffer[16];					//	1�s�N�Z���̃r�b�g��
		const int lineByteSize = width * pixcelDepth / 8;	//	1�s�̃o�C�g��
		uint8_t* top = buffer.data() + tgaHeaderSize;		//	��̍s�̈ʒu
		uint8_t* bottom = top + lineByteSize * (height - 1);//	���̍s�̈ʒu
		std::vector<uint8_t> tmp(lineByteSize);				//	����ւ��p�o�b�t�@

		//	�㉺�̍s������ւ��܂ŌJ��Ԃ�
		while (top < bottom) {
			std::copy_n(top, lineByteSize, tmp.data());		//	�u��̍s�̃R�s�[�v�����
			std::copy_n(bottom, lineByteSize, top);			//	���̍s����̍s�ɏ㏑��
			std::copy_n(tmp.data(), lineByteSize, bottom);	//	�u��̍s�̃R�s�[�v�����̍s�ɏ㏑��
			top += lineByteSize;							//	��̍s��1�s���Ɉړ�
			bottom -= lineByteSize;							//	���̍s��1�s��Ɉړ�
		}
	}


	//	�e�N�X�`�����쐬
	GLuint object = 0;		//	�Ǘ��ԍ�
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
 *	�f�X�g���N�^
 */
Texture::~Texture() {
	glDeleteTextures(1, &id);
}

