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
	vao->SetAttribute(2, 3, sizeof(Vertex), offsetof(Vertex, normal));

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
 *	MTLファイルを読み込む
 *	@param	foldername
 *	@param	filename
 *	@return		MTLファイルに含まれるマテリアル配列
 */
std::vector<MaterialPtr> LoadMTL(const std::string& foldername, const char* filename) {
	//	MTLファイルを開く
	const std::string fullpath = foldername + filename;
	std::ifstream file(fullpath);
	if (!file)
		return {};	//	読み込み失敗

	//	MTLファイルを解析する
	std::vector<MaterialPtr> materials;
	MaterialPtr pMat;

	while (!file.eof()) {
		std::string line;
		std::getline(file, line);
		const char* p = line.c_str();

		//	マテリアル定義の読み取りを試みる
		char name[1000] = { 0 };
		if (sscanf_s(line.data(), " newmtl %999s", name, 999) == 1) {
			pMat = std::make_shared<Material>();
			pMat->name = name;
			materials.push_back(pMat);
			continue;
		}

		//	マテリアルが定義されていない場合は処理しない
		if (!pMat)
			continue;

		//	基本色の読み取りを試みる
		if (sscanf_s(line.data(), " Kd %f %f %f",
			&pMat->baseColor.x, &pMat->baseColor.y, &pMat->baseColor.z) == 3) {
			continue;
		}

		//	不透明度の読み取りを試みる
		if (sscanf_s(line.data(), " d %f", &pMat->baseColor.w) == 1) {
			continue;
		}

		//	基本色テクスチャ名の読み取りを試みる
		char texName[1000] = { 0 };
		if (sscanf_s(line.data(), " map_Kd %999s", &texName, 999) == 1) {
			const std::string filename = foldername + texName;
			if (std::filesystem::exists(filename)) {
				pMat->texBaseColor = std::make_shared<Texture>(filename.c_str());
			}
			else {
				//	ファイルが開けない
			}
			continue;
		}

		//	発光色の読み取りを試みる
		if (sscanf_s(line.data(), " Ke %f %f %f",
			&pMat->emission.x, &pMat->emission.y, &pMat->emission.z) == 3){
			continue;
		}

		//	発光色テクスチャの読み取りを試みる
		if (sscanf_s(line.data(), " map_Ke %999s", &texName, 999) == 1) {
			const std::string filename = foldername + texName;
			if (std::filesystem::exists(filename)) {
				pMat->texEmission = std::make_shared<Texture>(filename.c_str());
			}
			else {
				//	ファイルが開けない
			}
			continue;
		}
	}

	//	読み込んだマテリアルを返す
	return materials;
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


	//	MTLファイルを開くためのフォルダ名を取得する
	std::string foldername(filename);
	{
		const size_t p = foldername.find_last_of("\\/");
		if (p != std::string::npos) {
			foldername.resize(p + 1);
		}
	}

	//	OBJファイルを解析して、頂点データとインデックスデータを読み込む
	std::vector<Vector3> positions;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;
	struct IndexSet { int v, vt, vn; };
	std::vector<IndexSet> faceIndexSet;

	positions.reserve(20'000);
	texcoords.reserve(20'000);
	normals.reserve(20'000);
	faceIndexSet.reserve(20'000 * 3);

	//	マテリアル
	std::vector<MaterialPtr> materials;
	materials.reserve(100);

	//	マテリアルの使用範囲
	struct UseMaterial {
		std::string name;		//	マテリアル名
		size_t startOffset;		//	割り当てする範囲の先頭位置
	};
	std::vector<UseMaterial> usemtls;
	usemtls.reserve(100);

	//	仮データを挿入しておく（マテリアルの指定がないOBLファイル対策）
	usemtls.push_back({ "", 0 });


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

		//	法線の読み取りを試みる
		Vector3 vn;
		if (sscanf_s(p, " vn %f %f %f", &vn.x, &vn.y, &vn.z) == 3) {
			normals.push_back(vn);
			continue;
		}

		//	インデックスデータの読み取りを試みる
		IndexSet f0, f1, f2;
		int readByte;
		//	頂点座標 + テクスチャ座標 + 法線
		if (sscanf_s(p, " f %u/%u/%u %u/%u/%u%n",
			&f0.v, &f0.vt, &f0.vn, &f1.v, &f1.vt, &f1.vn, &readByte) == 6) {
			p += readByte;	//	読み取り位置を更新
			while (1) {
				if (sscanf_s(p, " %u/%u/%u%n", &f2.v, &f2.vt, &f2.vn, &readByte) != 3)
					break;
				p += readByte;	//	読み取り位置を更新
				faceIndexSet.push_back(f0);
				faceIndexSet.push_back(f1);
				faceIndexSet.push_back(f2);
				f1 = f2;	//	次の三角形のためにデータを移動
			}
			continue;
		}

		//	頂点座標 + テクスチャ座標
		if (sscanf_s(p, " f %u/%u %u/%u%n",
			&f0.v, &f0.vt, &f1.v, &f1.vt, &readByte) == 4) {
			f0.vn = f1.vn = 0;	//	法線なし
			p += readByte;	//	読み取り位置を更新
			while (1) {
				if (sscanf_s(p, " %u/%u%n", &f2.v, &f2.vt, &readByte) != 2)
					break;

				f2.vn = 0;		//	法線なし
				p += readByte;	//	読み取り位置を更新
				faceIndexSet.push_back(f0);
				faceIndexSet.push_back(f1);
				faceIndexSet.push_back(f2);
				f1 = f2;	//	次の三角形のためにデータを移動
			}
			continue;
		}

		//	MTLファイルの読み取りを試みる
		char mtlFilename[1000] = { 0 };
		if (sscanf_s(line.data(), " mtllib %999s", mtlFilename, 999) == 1) {
			const auto tmp = LoadMTL(foldername, mtlFilename);
			materials.insert(materials.end(), tmp.begin(), tmp.end());
			continue;
		}

		//	使用するマテリアル名の読み取りを試みる
		char mtlName[1000];
		if (sscanf_s(line.data(), " usemtl %999s", mtlName, 999) == 1) {
			usemtls.push_back({ mtlName, faceIndexSet.size() });
			continue;
		}
	}

	//	使用するマテリアルの末尾に番兵を追加
	usemtls.push_back({ "", faceIndexSet.size() });

	//	読み込んだデータをOpenGLで使えるデータに変換する
	std::vector<Vertex> vertices;
	vertices.reserve(faceIndexSet.size());
	for (const auto& face : faceIndexSet) {
		Vertex v;
		v.position = positions[face.v - 1];
		v.texCoord = texcoords[face.vt - 1];
		//	法線が設定されていない場合は0を設定
		if (face.vn == 0) {
			v.normal = Vector3::zero;
		}
		else {
			v.normal = normals[face.vn - 1];
		}

		vertices.push_back(v);
	}

	std::vector<uint16_t> indices(faceIndexSet.size());
	for (int i = 0; i < indices.size(); i++) {
		indices[i] = i;
	}

	//	設定されていない法線を補う
	FillMissingNormals(vertices.data(), vertices.size(),
		indices.data(), indices.size());

	//	変換したデータをバッファに追加
	AddVertexData(
		vertices.data(), vertices.size() * sizeof(Vertex),
		indices.data(), indices.size() * sizeof(uint16_t)
	);

	//	メッシュを生成
	auto pMesh = std::make_shared<StaticMesh>();

	//	データの位置を取得
	const void* indexOffset = drawParamList.back().indices;
	const GLint baseVertex = drawParamList.back().baseVertex;

	//	マテリアルに対応した描画パラメータを生成
	//	仮データ、番兵のデータ以外のマテリアルある場合は仮データを飛ばす
	size_t i = 0;
	if (usemtls.size() > 2) {
		i = 1;
	}

	for (; i < usemtls.size() - 1; i++) {
		const UseMaterial& current = usemtls[i];		//	現在のマテリアル
		const UseMaterial& next = usemtls[i + 1];		//	次のマテリアル
		//	インデックスデータがない場合は処理しない
		if (next.startOffset == current.startOffset)
			continue;

		//	描画パラメータを作成
		DrawParam param;
		param.mode = GL_TRIANGLES;
		param.count = static_cast<GLsizei>(next.startOffset - current.startOffset);
		param.indices = indexOffset;
		param.baseVertex = baseVertex;
		param.materialNo = 0;		//	デフォルト値を設定
		//	名前の一致するマテリアルを設定
		for (int i = 0; i < materials.size(); i++) {
			if (materials[i]->name == current.name) {
				param.materialNo = i;
				break;
			}
		}
		pMesh->drawParamList.push_back(param);

		//	インデックスオフセットを変更
		indexOffset = reinterpret_cast<void*>(
			reinterpret_cast<size_t>(indexOffset) + sizeof(uint16_t) * param.count
			);
	}

	//	マテリアル配列が空の場合はデフォルトのマテリアルを追加
	if (materials.empty()) {
		pMesh->materials.push_back(std::make_shared<Material>());
	}
	else {
		pMesh->materials.assign(materials.begin(), materials.end());
	}

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
void Draw(const StaticMesh& mesh, GLuint prog) {
	//	カラーパラメータを取得
	Vector4 objectColor;
	if (prog) {
		glGetUniformfv(prog, 100, &objectColor.x);
	}

	for (const auto& drawParam : mesh.drawParamList) {
		//	マテリアルを設定
		if (drawParam.materialNo >= 0 && drawParam.materialNo < mesh.materials.size()) {
			const Material& mat = *mesh.materials[drawParam.materialNo];

			if (prog) {
				const Vector4 color = Vector4::Scale(objectColor, mat.baseColor);
				//color = {
				//	objectColor.x* mat.baseColor.x,
				//	objectColor.y* mat.baseColor.y,
				//	objectColor.z* mat.baseColor.z,
				//	objectColor.w* mat.baseColor.w
				//};
				glProgramUniform4fv(prog, 100, 1, &color.x);
				glProgramUniform4f(prog, 101,
					mat.emission.x, mat.emission.y, mat.emission.z,
					static_cast<bool>(mat.texEmission)
				);

			}

			if (mat.texBaseColor) {
				const GLuint tex = *mat.texBaseColor;
				glBindTextures(0, 1, &tex);
			}

			if (mat.texEmission) {
				const GLuint tex = *mat.texEmission;
				glBindTextures(1, 1, &tex);
			}
			else {
				glBindTextures(1, 1, nullptr);
			}
		}

		glDrawElementsBaseVertex(
			drawParam.mode,
			drawParam.count,
			GL_UNSIGNED_SHORT,
			drawParam.indices,
			drawParam.baseVertex
		);
	}
}

/*
 *	欠けている法線を補う
 *	@param	vertices		頂点配列
 *	@param	vertexCount		頂点配列の要素数
 *	@param	indices			インデックス配列
 *	@param	indexCount		インデックス配列の要素数
 */
void FillMissingNormals(Vertex* vertices, size_t vertexCount, const uint16_t* indices, size_t indexCount) {
	//	法線が設定されていない頂点を見つける
	std::vector<bool> missingNormals(vertexCount, false);
	for (int i = 0; i < vertexCount; i++) {
		//	法線の長さが0の場合、「設定されていない」とみなす
		const Vector3& n = vertices[i].normal;
		if (n == Vector3::zero) {
			missingNormals[i] = true;
		}
	}
	//	法線を計算
	for (int i = 0; i < indexCount; i += 3) {
		//	面を構成する3点から辺a, b を求める
		const int i0 = indices[i + 0];
		const int i1 = indices[i + 1];
		const int i2 = indices[i + 2];
		const Vector3& v0 = vertices[i0].position;
		const Vector3& v1 = vertices[i1].position;
		const Vector3& v2 = vertices[i2].position;
		const Vector3 a = v1 - v0;
		const Vector3 b = v2 - v0;

		//	外積によって aとbに垂直なベクトル(法線)を求める
		const Vector3 normal = Vector3::Cross(a, b);

		//	法線が設定されていない頂点に加算
		if (missingNormals[i0])
			vertices[i0].normal += normal;
		if (missingNormals[i1])
			vertices[i1].normal += normal;
		if (missingNormals[i2])
			vertices[i2].normal += normal;
	}

	//	法線を正規化
	for (int i = 0; i < vertexCount; i++) {
		if (missingNormals[i]) {
			Vector3& n = vertices[i].normal;
			n = n.Normalized();
		}
	}
}
