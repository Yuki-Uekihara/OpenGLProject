/*
 *	@file	Mesh.cpp
 */ 

#include "Mesh.h"
#include <numeric>

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

	//	描画パラメータの容量を予約
	drawParamList.reserve(100);
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
	drawParamList.clear();
}
