/*
 *	@file	Mesh.cpp
 */

#include "Mesh.h"
#include <numeric>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

 //	コンストラクタ
MeshBuffer::MeshBuffer(size_t bufferSize) {
	//	バッファオブジェクトを生成
	buffer = BufferObject::Create(bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

	//	VAOを生成
	vao = VertexArrayObject::Create();

	glBindVertexArray(*vao);
	//	VBOをVAOにバインド
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
	//	IBOをVAOにバインド
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);

	//	頂点アトリビュートの設定
	vao->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
	vao->SetAttribute(1, 2, sizeof(Vertex), offsetof(Vertex, texCoord));

	//	誤操作防止用
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//	スタティックメッシュの容量を予約
	meshes.reserve(100);
	//	描画パラメータの容量を予約
	drawParamList.reserve(100);
}

/*

*	OBJファイルを読み込む
 *	@param	filename
 *	@return	生成したメッシュ
 */
StaticMeshPtr MeshBuffer::LoadOBJ(const char* filename) {
	//	以前に読み込んでいるファイルの場合、作成済みのものを返す
	auto itr = meshes.find(filename);
	if (itr != meshes.end())
		return itr->second;

	//	OBJファイルを開く
	std::ifstream file(filename);
	if (!file)
		return nullptr;		//	読み込み失敗

	//	OBJファイルを解析して、頂点データとインデックスデータを読み込む
	std::vector<Vector3> positions;
	std::vector<Vector2> texcoords;
	struct IndexSet { int v, vt; };
	std::vector<IndexSet> faceIndexSet;

	positions.reserve(20'000);
	texcoords.reserve(20'000);
	faceIndexSet.reserve(20'000 * 3);

	while (!file.eof()) {
		std::string line;
		std::getline(file, line);
		const char* p = line.c_str();

		//	頂点座標の読み取りを試みる
		Vector3 v;
		if (sscanf_s(p, " v %f %f %f", &v.x, &v.y, &v.z) == 3) {
			positions.push_back(v);
			continue;
		}

		//	テクスチャ座標の読み取りを試みる
		Vector2 vt;
		if (sscanf_s(p, " vt %f %f", &vt.x, &vt.y) == 2) {
			texcoords.push_back(vt);
			continue;
		}

		//	インデックスデータの読み取りを試みる
		IndexSet f0, f1, f2;
		int readByte;
		if (sscanf_s(p, " f %u/%u %u/%u%n",
			&f0.v, &f0.vt, &f1.v, &f1.vt, &readByte) == 4) {
			p += readByte;	//	読み取り位置を更新
			while (1) {
				if (sscanf_s(p, " %u/%u%n", &f2.v, &f2.vt, &readByte) != 2)
					break;
				p += readByte;	//	読み取り位置を更新
				faceIndexSet.push_back(f0);
				faceIndexSet.push_back(f1);
				faceIndexSet.push_back(f2);
				f1 = f2;	//	次の三角形のためにデータを移動
			}
			continue;
		}
	}

	//	読み込んだデータをOpenGLで使えるデータに変換する
	std::vector<Vertex> vertices;
	vertices.reserve(faceIndexSet.size());
	for (const auto& face : faceIndexSet) {
		Vertex v;
		v.position = positions[face.v - 1];
		v.texCoord = texcoords[face.vt - 1];
		vertices.push_back(v);
	}

	std::vector<uint16_t> indices(faceIndexSet.size());
	for (int i = 0; i < indices.size(); i++) {
		indices[i] = i;
	}

	//	変換したデータをバッファに追加
	AddVertexData(
		vertices.data(), vertices.size() * sizeof(Vertex),
		indices.data(), indices.size() * sizeof(uint16_t)
	);

	//	メッシュを生成
	auto pMesh = std::make_shared<StaticMesh>();
	pMesh->drawParamList.push_back(drawParamList.back());
	pMesh->name = filename;
	meshes.emplace(filename, pMesh);
	//	生成したメッシュを返す
	return pMesh;
}

/*
 *	頂点データの追加
 *	@param	vertices	 GPUメモリにコピーする頂点データ配列
 *	@param	vertexBytes	 verticesのバイト数
 *	@param	indices		 GPUメモリにコピーするインデックスデータ配列
 *	@param	indexBytes	 indicesのバイト数
 *	@param	mode		 プリミティブの種類
 */
void MeshBuffer::AddVertexData(const Vertex* vertices, size_t vertexBytes, const uint16_t* indices, size_t indexBytes, GLenum mode) {
	//	空き容量の確認	必要なバイト数が空きバイト数より大きい場合は追加できない
	const size_t totalBytes = vertexBytes + indexBytes;
	const size_t freeBytes = buffer->GetSize() - usedBytes;

	if (totalBytes > freeBytes)
		return;		//	容量が足りない

	//	頂点データをGPUメモリにコピー
	GLuint tmp[2];	//	バッファ用一時変数
	glCreateBuffers(2, tmp);
	//	GPUのメモリを確保
	glNamedBufferStorage(tmp[0], vertexBytes, vertices, 0);
	glNamedBufferStorage(tmp[1], indexBytes, indices, 0);
	//	データをコピー
	glCopyNamedBufferSubData(tmp[0], *buffer, 0, usedBytes, vertexBytes);
	glCopyNamedBufferSubData(tmp[1], *buffer, 0, usedBytes + vertexBytes, indexBytes);
	glDeleteBuffers(2, tmp);	//	一時バッファを削除

	//	描画パラメータを作成
	DrawParam newParam;
	newParam.mode = mode;
	newParam.count = static_cast<GLsizei>(indexBytes / sizeof(uint16_t));
	newParam.indices = reinterpret_cast<void*>(usedBytes + vertexBytes);
	newParam.baseVertex = static_cast<GLint>(usedBytes / sizeof(Vertex));
	drawParamList.push_back(newParam);		//	描画パラメータを追加

	//	次のデータ格納位置を計算
	constexpr size_t a = std::lcm(sizeof(uint16_t), sizeof(Vertex));	// least common multiple	最小公倍数を求める
	usedBytes += ((totalBytes + a - 1) / a) * a;
}

/*
 *	全ての頂点データを削除
 */
void MeshBuffer::Clear() {
	usedBytes = 0;
	meshes.clear();
	drawParamList.clear();
}

/*
 *	スタティックメッシュの取得
 *	@param	name
 *	@return	StaticMesh
 */
StaticMeshPtr MeshBuffer::GetStaticMesh(const char* name) const {
	auto itr = meshes.find(name);
	if (itr != meshes.end()) {
		return itr->second;
	}
	return nullptr;
}

/*
 *	メッシュを描画する
 */
void Draw(const StaticMesh& mesh) {
	for (const auto& drawParam : mesh.drawParamList) {
		glDrawElementsBaseVertex(
			drawParam.mode,
			drawParam.count,
			GL_UNSIGNED_SHORT,
			drawParam.indices,
			drawParam.baseVertex
		);
	}
}
