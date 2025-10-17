/*
 *	@file	BufferObject.h
 */

#ifndef _BUFFEROBJECT_H_
#define _BUFFEROBJECT_H_

#include "glad/glad.h"
#include <memory>

/*
 *	�o�b�t�@�I�u�W�F�N�g(BO) 
 */
class BufferObject {
private:
	GLuint id = 0;					//	�I�u�W�F�N�g�Ǘ��ԍ�
	GLsizeiptr bufferSize = 0;		//	�o�b�t�@�T�C�Y(byte)

public:
	BufferObject(GLsizeiptr size, const void* data, GLbitfield flags)
		: bufferSize(size) {
		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, bufferSize, data, flags);
	}

	~BufferObject() {
		glDeleteBuffers(1, &id);
	}

	//	�R�s�[�Ƒ���̋֎~
	BufferObject(const BufferObject&) = delete;
	BufferObject& operator = (const BufferObject&) = delete;

public:
	/*
	 *	�o�b�t�@�I�u�W�F�N�g�𐶐�����
	 *	@param	size	�o�b�t�@�I�u�W�F�N�g�̃o�C�g�T�C�Y
	 *	@param	data	�o�b�t�@�ɃR�s�[����f�[�^�̃A�h���X
	 *	@param	flags	�r�b�g�t���O�̘_���a
	 *	@return			�쐬�����o�b�t�@�I�u�W�F�N�g�̃|�C���^
	 */
	static std::shared_ptr<BufferObject> Create(GLsizeiptr size, const void* data, GLbitfield flags) {
		return std::make_shared<BufferObject>(size, data, flags);
	}

public:
	//	�Ǘ��ԍ��̎擾
	inline operator GLuint() const { return id; }

	//	�o�b�t�@�T�C�Y�̎擾
	inline GLsizeiptr GetSize() const { return bufferSize; }
};
//	�ʖ���`
using BufferObjectPtr = std::shared_ptr<BufferObject>;


#endif // !_BUFFEROBJECT_H_
