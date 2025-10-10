/*
 *	@file	Texture.h
 */

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "glad/glad.h"
#include <string>
#include <memory>

/*
 *	�e�N�X�`���Ǘ��N���X
 */
class Texture {
private:
	std::string name;			//	�e�N�X�`����(�f�o�b�O�p)
	GLuint id = 0;				//	�Ǘ��ԍ�
	int width = 0, height = 0;	//	���ƍ���

public:
	explicit Texture(const char* filename);
	~Texture();
	//	�R�s�[�Ƒ���̋֎~
	Texture(const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;

public:
	//	���O�̎擾
	inline const std::string& GetName() const { return name; }

	//	�Ǘ��ԍ��̎擾
	operator GLuint() const { return id; }

	//	���̎擾
	inline int GetWidth() const { return width; }

	//	�����̎擾
	inline int GetHeight() const { return height; }

	//	�e�N�X�`���̃A�X�y�N�g����擾
	inline float GetAspectRatio() const {
		return static_cast<float>(width) / static_cast<float>(height);
	}
};
//	�ʖ���`
using TexturePtr = std::shared_ptr<Texture>;

#endif // !_TEXTURE_H_
