/*
 *	@file	Mesh.h
 */

#ifndef _MESH_H_
#define _MESH_H_

#include "glad/glad.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "VecMath.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

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
 *	3Dモデル 
 */
struct StaticMesh {
	std::string name;		//	メッシュ名
	std::vector<DrawParam> drawParamList;	//	描画パラメータ
};
//	別名定義
using StaticMeshPtr = std::shared_ptr<StaticMesh>;

void Draw(const StaticMesh& mesh);


/*
 *	頂点データ
 */	
struct Vertex {
	Vector3 position;		//	頂点座標
	Vector2 texCoord;		//	テクスチャ座標
};


/*
 *	頂点データを管理するクラス
 */
class MeshBuffer {
private:
	std::unordered_map<std::string, StaticMeshPtr> meshes;
	std::vector<DrawParam> drawParamList;	//	描画パラメータ配列
	VertexArrayObjectPtr vao;				//	頂点アトリビュート配列
	BufferObjectPtr buffer;					//	頂点データ、インデックスデータ
	size_t usedBytes = 0;					//	バッファの使用済み容量(byte)

public:
	MeshBuffer(size_t bufferSize);
	~MeshBuffer() = default;

	//	コピーと代入の禁止
	MeshBuffer(const MeshBuffer&) = delete;
	MeshBuffer& operator = (const MeshBuffer&) = delete;

public:
	/*
	 *	メッシュバッファを生成する
	 *	@param	bufferSize
	 */
	static std::shared_ptr<MeshBuffer> Create(size_t bufferSize) {
		return std::make_shared<MeshBuffer>(bufferSize);
	}

public:
	/*
	 *	OBJファイルを読み込む
	 *	@param	filename
	 *	@return	生成したメッシュ
	 */
	StaticMeshPtr LoadOBJ(const char* filename);

	/*
	 *	頂点データの追加
	 *	@param	vertices	 GPUメモリにコピーする頂点データ配列
	 *	@param	vertexBytes	 verticesのバイト数
	 *	@param	indices		 GPUメモリにコピーするインデックスデータ配列
	 *	@param	indexBytes	 indicesのバイト数
	 *	@param	mode		 プリミティブの種類
	 */
	void AddVertexData(const Vertex* vertices, size_t vertexBytes,
		const uint16_t* indices, size_t indexBytes, GLenum mode = GL_TRIANGLES);

	//	全ての頂点データを削除
	void Clear();

public:
	/*
	 *	スタティックメッシュの取得
	 *	@param	name
	 *	@return	StaticMesh
	 */
	StaticMeshPtr GetStaticMesh(const char* name) const;

	//	描画パラメータ数の取得
	inline size_t GetDrawParamSize() const { return drawParamList.size(); }

	//	描画パラメータの取得
	inline const DrawParam& GetDrawParam(size_t index) const { return drawParamList[index]; }

	//	VAOの取得
	inline VertexArrayObjectPtr GetVAO() const { return vao; }
};
//	別名定義
using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

#endif // !_MESH_H_
