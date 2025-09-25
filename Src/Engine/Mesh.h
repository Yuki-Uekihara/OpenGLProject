/*
 *	@file	Mesh.h
 */

#ifndef _MESH_H_
#define _MESH_H_

#include "glad/glad.h"
#include "VecMath.h"

/*
 *	�`��p�����[�^
 */
struct DrawParam {
	GLenum mode = GL_TRIANGLES;		//	�v���~�e�B�u�̎��
	GLsizei count = 0;				//	�`�悷��C���f�b�N�X��
	const void* indices;			//	�`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
	GLint baseVertex = 0;			//	�C���f�b�N�X0�ԂƂȂ�ʒu
};

/*
 *	���_�f�[�^
 */	
struct Vertex {
	Vector3 position;		//	���_���W
	Vector2 texCoord;		//	�e�N�X�`�����W
};


#endif // !_MESH_H_
