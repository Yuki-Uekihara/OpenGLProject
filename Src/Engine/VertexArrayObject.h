/*
 *	@file	VertexArrayObject.h 
 */

#ifndef _VERTEXARRAYOBJECT_H_
#define _VERTEXARRAYOBJECT_H_

#include "glad/glad.h"
#include <memory>

/*
 *	頂点配列オブジェクト(VAO)
 */
class VertexArrayObject {
private:
	GLuint id = 0;		//	オブジェクト管理番号

public:
	VertexArrayObject() {
		glCreateVertexArrays(1, &id);
	}

	~VertexArrayObject() {
		glDeleteVertexArrays(1, &id);
	}

	//	コピーと代入の禁止
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator = (const VertexArrayObject&) = delete;

public:
	/*
	 *	VAOを生成する
	 */
	static std::shared_ptr<VertexArrayObject> Create() {
		std::make_shared<VertexArrayObject>();
	}

public:
	/*
	 *	頂点アトリビュートの設定
	 *	@param	index	設定する頂点アトリビュートの番号(0~15)
	 *	@param	size	要素のデータ数 (float=1, Vector2=2, Vector3=3, Vector4=4)
	 *	@param	stride	次の頂点データにある同一の要素までの間隔(byte)
	 *	@param	offset	頂点データ内における要素の位置(byte)
	 */
	void SetAttribute(GLuint index, GLint size, size_t stride, size_t offset) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
			static_cast<GLsizei>(stride), reinterpret_cast<void*>(offset));
	}

public:
	//	管理番号の取得
	inline operator GLuint() const { return id; }
};
//	別名定義
using VertexArrayObjectPtr = std::shared_ptr<VertexArrayObject>;

#endif // !_VERTEXARRAYOBJECT_H_
