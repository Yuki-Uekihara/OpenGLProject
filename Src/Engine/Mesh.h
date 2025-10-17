/*
 *	@file	Mesh.h
 */

#ifndef _MESH_H_
#define _MESH_H_

#include "glad/glad.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
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


/*
 *	���_�f�[�^���Ǘ�����N���X
 */
class MeshBuffer {
private:
	std::vector<DrawParam> drawParamList;	//	�`��p�����[�^�z��
	VertexArrayObjectPtr vao;				//	���_�A�g���r���[�g�z��
	BufferObjectPtr buffer;					//	���_�f�[�^�A�C���f�b�N�X�f�[�^
	size_t usedBytes = 0;					//	�o�b�t�@�̎g�p�ςݗe��(byte)

public:
	MeshBuffer(size_t bufferSize);
	~MeshBuffer() = default;

	//	�R�s�[�Ƒ���̋֎~
	MeshBuffer(const MeshBuffer&) = delete;
	MeshBuffer& operator = (const MeshBuffer&) = delete;

public:
	/*
	 *	���b�V���o�b�t�@�𐶐�����
	 *	@param	bufferSize
	 */
	static std::shared_ptr<MeshBuffer> Create(size_t bufferSize) {
		std::make_shared<MeshBuffer>(bufferSize);
	}

public:
	/*
	 *	���_�f�[�^�̒ǉ�
	 *	@param	vertices	 GPU�������ɃR�s�[���钸�_�f�[�^�z��
	 *	@param	vertexBytes	 vertices�̃o�C�g��
	 *	@param	indices		 GPU�������ɃR�s�[����C���f�b�N�X�f�[�^�z��
	 *	@param	indexBytes	 indices�̃o�C�g��
	 *	@param	mode		 �v���~�e�B�u�̎��
	 */
	void AddVertexData(const Vertex* vertices, size_t vertexBytes,
		const uint16_t* indices, size_t indexBytes, GLenum mode = GL_TRIANGLES);

	//	�S�Ă̒��_�f�[�^���폜
	void Clear();

public:
	//	�`��p�����[�^���̎擾
	inline size_t GetDrawParamSize() const { return drawParamList.size(); }

	//	VAO�̎擾
	inline VertexArrayObjectPtr GetVAO() const { return vao; }
};
//	�ʖ���`
using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

#endif // !_MESH_H_
