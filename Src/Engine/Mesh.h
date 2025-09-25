/*
 *	@file	Mesh.h
 */

#ifndef _MESH_H_
#define _MESH_H_

#include "glad/glad.h"
#include "VecMath.h"

/*
 *	描画パラメータ
 */
struct DrawParam {
	GLenum mode = GL_TRIANGLES;		//	プリミティブの種類
	GLsizei count = 0;				//	描画するインデックス数
	const void* indices;			//	描画開始インデックスのバイトオフセット
	GLint baseVertex = 0;			//	インデックス0番となる位置
};

/*
 *	頂点データ
 */	
struct Vertex {
	Vector3 position;		//	頂点座標
	Vector2 texCoord;		//	テクスチャ座標
};


#endif // !_MESH_H_
