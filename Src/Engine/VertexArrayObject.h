/*
 *	@file	VertexArrayObject.h 
 */

#ifndef _VERTEXARRAYOBJECT_H_
#define _VERTEXARRAYOBJECT_H_

#include "glad/glad.h"
#include <memory>

/*
 *	���_�z��I�u�W�F�N�g(VAO)
 */
class VertexArrayObject {
private:
	GLuint id = 0;		//	�I�u�W�F�N�g�Ǘ��ԍ�

public:
	VertexArrayObject() {
		glCreateVertexArrays(1, &id);
	}

	~VertexArrayObject() {
		glDeleteVertexArrays(1, &id);
	}

	//	�R�s�[�Ƒ���̋֎~
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator = (const VertexArrayObject&) = delete;

public:
	/*
	 *	VAO�𐶐�����
	 */
	static std::shared_ptr<VertexArrayObject> Create() {
		std::make_shared<VertexArrayObject>();
	}

public:
	/*
	 *	���_�A�g���r���[�g�̐ݒ�
	 *	@param	index	�ݒ肷�钸�_�A�g���r���[�g�̔ԍ�(0~15)
	 *	@param	size	�v�f�̃f�[�^�� (float=1, Vector2=2, Vector3=3, Vector4=4)
	 *	@param	stride	���̒��_�f�[�^�ɂ��铯��̗v�f�܂ł̊Ԋu(byte)
	 *	@param	offset	���_�f�[�^���ɂ�����v�f�̈ʒu(byte)
	 */
	void SetAttribute(GLuint index, GLint size, size_t stride, size_t offset) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
			static_cast<GLsizei>(stride), reinterpret_cast<void*>(offset));
	}

public:
	//	�Ǘ��ԍ��̎擾
	inline operator GLuint() const { return id; }
};
//	�ʖ���`
using VertexArrayObjectPtr = std::shared_ptr<VertexArrayObject>;

#endif // !_VERTEXARRAYOBJECT_H_
