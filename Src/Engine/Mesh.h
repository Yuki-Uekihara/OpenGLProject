/*
 *	@file	Mesh.h
 */

#ifndef _MESH_H_
#define _MESH_H_

#include "glad/glad.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "Texture.h"
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
	int materialNo = -1;			//	マテリアルインデックス
};


/*
 *	頂点データ
 */
struct Vertex {
	Vector3 position;		//	頂点座標
	Vector2 texCoord;		//	テクスチャ座標
	Vector3 normal;			//	法線ベクトル
};

/*
 *	マテリアル
 */
struct Material {
	std::string name = "<Default>";		//	マテリアル名
	Vector4 baseColor = Vector4::one;	//	基本色 + α値
	Vector3 emission = Vector3::zero;	//	発光色
	TexturePtr texBaseColor;			//	基本色テクスチャ
	TexturePtr texEmission;				//	発光色テクスチャ
};
//	別名定義
using MaterialPtr = std::shared_ptr<Material>;
using MaterialList = std::vector<MaterialPtr>;

/*
 *	3Dモデル
 */
struct StaticMesh {
	std::string name;		//	メッシュ名
	std::vector<DrawParam> drawParamList;	//	描画パラメータ
	MaterialList materials;		//	マテリアル配列
};
//	別名定義
using StaticMeshPtr = std::shared_ptr<StaticMesh>;

void Draw(const StaticMesh& mesh, GLuint prog, const MaterialList& materials);

//	共有マテリアル配列を複製する
inline MaterialList CloneMaterialList(const StaticMeshPtr& original) {
	MaterialList clone(original->materials.size());
	for (int i = 0; i < clone.size(); i++) {
		clone[i] = std::make_shared<Material>(*original->materials[i]);
	}
	return clone;
}

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


	/*
	 *	既存の描画パラメータとテクスチャから新しいスタティックメッシュを生成する
	 *	@param	name			メッシュ名
	 *	@param	param			メッシュの元になる描画データ
	 *	@param	texBaseColor	メッシュに設定するベースカラーテクスチャ
	 *	@return					生成したスタティックメッシュ
	 */
	StaticMeshPtr CreateStaticMesh(const char* name, const DrawParam& param, const TexturePtr& texBaseColor);

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

/*
 *	欠けている法線を補う
 *	@param	vertices		頂点配列
 *	@param	vertexCount		頂点配列の要素数
 *	@param	indices			インデックス配列
 *	@param	indexCount		インデックス配列の要素数
 */
void FillMissingNormals(
	Vertex* vertices, size_t vertexCount,
	const uint16_t* indices, size_t indexCount);

#endif // !_MESH_H_
