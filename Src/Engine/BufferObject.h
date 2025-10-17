/*
 *	@file	BufferObject.h
 */

#ifndef _BUFFEROBJECT_H_
#define _BUFFEROBJECT_H_

#include "glad/glad.h"
#include <memory>

/*
 *	バッファオブジェクト(BO) 
 */
class BufferObject {
private:
	GLuint id = 0;					//	オブジェクト管理番号
	GLsizeiptr bufferSize = 0;		//	バッファサイズ(byte)

public:
	BufferObject(GLsizeiptr size, const void* data, GLbitfield flags)
		: bufferSize(size) {
		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, bufferSize, data, flags);
	}

	~BufferObject() {
		glDeleteBuffers(1, &id);
	}

	//	コピーと代入の禁止
	BufferObject(const BufferObject&) = delete;
	BufferObject& operator = (const BufferObject&) = delete;

public:
	/*
	 *	バッファオブジェクトを生成する
	 *	@param	size	バッファオブジェクトのバイトサイズ
	 *	@param	data	バッファにコピーするデータのアドレス
	 *	@param	flags	ビットフラグの論理和
	 *	@return			作成したバッファオブジェクトのポインタ
	 */
	static std::shared_ptr<BufferObject> Create(GLsizeiptr size, const void* data, GLbitfield flags) {
		return std::make_shared<BufferObject>(size, data, flags);
	}

public:
	//	管理番号の取得
	inline operator GLuint() const { return id; }

	//	バッファサイズの取得
	inline GLsizeiptr GetSize() const { return bufferSize; }
};
//	別名定義
using BufferObjectPtr = std::shared_ptr<BufferObject>;


#endif // !_BUFFEROBJECT_H_
