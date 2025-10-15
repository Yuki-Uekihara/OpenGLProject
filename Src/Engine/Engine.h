/*
 *	@file	Engine.h
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <utility>	//	std::pair

#include "GameObject.h"
#include "Scene.h"
#include "Mesh.h"


//	メッシュ番号
//	Initialize関数にある meshes 配列と順番を合わせる
//	配列に変更があった場合はこちらも変更する
enum MeshId {
	MeshId_box,
	MeshId_crystal,
	MeshId_wall,
	MeshId_plane_xy
};


/*
 *	ゲームエンジン
 */
class Engine {
private:
	GLFWwindow* window = nullptr;				//	ウィンドウオブジェクト
	const std::string title = "OpenGLGame";		//	ウィンドウのタイトル
	float degFovY = 60;							//	垂直視野角(度数法)
	float radFovY = degFovY * Rad2Deg;
	float fovScale = 1 / std::tanf(radFovY / 2);	//	視野角による拡大率の逆数
	
	GLuint vs = 0;
	GLuint fs = 0;
	GLuint prog = 0;
	GLuint vbo = 0;		//	vertex buffer object
	GLuint ibo = 0;		//	index buffer object
	GLuint vao = 0;		//	vertex array object

	GLintptr vboSize;	//	頂点バッファの現在のサイズ
	GLintptr iboSize;	//	インデックスバッファの現在のサイズ
	std::vector<DrawParam> drawParamList;	//	描画パラメータ配列

	GameObjectList gameObjects;		//	ゲームオブジェクトの一元管理
	float previousTime = 0;			//	前回の更新時
	float deltaTime = 0;			//	前回の更新からの経過時間

	ScenePtr scene;					//	実行中のシーン
	ScenePtr nextScene;				//	次のシーン


	GameObject camera;

	GLsizei indexCount = 0;

	//	ワールド座標系のコライダーを表す構造体
	struct WorldCollider {
		AABBColliderPtr origin;
		AABB world;
	};
	using WorldColliderList = std::vector<WorldCollider>;

public:
	Engine() = default;
	~Engine() = default;

	int Run();

private:
	int Initialize();
	void Update();
	void Render();

	void DrawGameObject(GameObjectList::iterator begin, GameObjectList::iterator end);
	void UpdateGameObject(float deltaTime);
	void HandleGameObjectCollision();
	void HandleWorldColliderCollision(WorldColliderList* a, WorldColliderList* b);
	void RemoveGameObject();
public:
	/*
	 *	ゲームオブジェクトを生成する
	 *	@tparam	ゲームオブジェクトまたはその派生クラス
	 *	@param	name
	 *	@param	position
	 *	@param	rotation
	 */
	template <class T>
	std::shared_ptr<T> Create(
		const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f }
	) {
		//	オブジェクトのポインタを生成
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position = position;
		p->rotation = rotation;
		//	一元管理に追加
		gameObjects.push_back(p);
		return p;
	}	

	/*
	 *	UIオブジェクトを作成する
	 *	@tparam	ゲームオブジェクトに割り当てるUILayoutまたはその派生クラス
	 *	@param	filename
	 *	@param	position
	 *	@param	scale
	 */
	template <class T>
	std::pair<GameObjectPtr, std::shared_ptr<T>> CreateUIObject(
		const char* filename, const Vector2& position, float scale
	) {
		//	メッセージオブジェクトを生成
		auto obj = Create<GameObject>(filename, { position.x, position.y, 0 });
		obj->texColor = std::make_shared<Texture>(filename);
		obj->meshId = MeshId_plane_xy;
		obj->renderQueue = RenderQueue_overlay;

		//	画像サイズに応じて拡大率を調整
		const float aspectRatio = obj->texColor->GetAspectRatio();
		obj->scale = { scale * aspectRatio, scale, 1.0f };

		//	コンポーネントの追加
		auto component = obj->AddComponent<T>();

		return { obj, component };
	}

	//	すべてのゲームオブジェクトを削除する
	void ClearGameObjects();

	//	次のシーンを設定する
	template <class T>
	void SetNextScene() { nextScene = std::make_shared<T>();  }

public:
	//	カメラを取得する
	inline GameObject& GetMainCamera() { return camera; }
	inline const GameObject& GetMainCamera() const { return camera; }

	//	キー入力の検知
	inline bool GetKey(int key) const {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	//	マウスカーソルの座標を取得する
	inline Vector2 GetMousePosition() const {
		//	スクリーン座標系のカーソル座標を取得
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		//	型を合わせる
		const Vector2 pos = { static_cast<float>(x), static_cast<float>(y) };

		//	UILayerの座標系と併せるために スクリーン座標系からカメラ座標系に変換
		//	カメラ座標系のマウス座標.x =
		//		((スクリーン座標系のマウス座標.x / 画面サイズ.x) * 2 - 1) * アスペクト比
		//	
		//	カメラ座標系のマウス座標.y =
		//		((スクリーン座標系のマウス座標.y / 画面サイズ.y) * 2 - 1) * -1

		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		const Vector2 framebufferSize = { static_cast<float>(w), static_cast<float>(h) };
		const float aspectRatio = framebufferSize.x / framebufferSize.y;

		return { (pos.x / framebufferSize.x * 2 - 1) * aspectRatio, (pos.y / framebufferSize.y * 2 - 1) * -1 };
	}

	//	マウスのボタンの状態を取得する
	inline bool GetMouseButton(int button) const {
		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	}

	//	フレームバッファのサイズを取得する
	inline Vector2 GetFramebufferSize() const {
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		return { static_cast<float>(w), static_cast<float>(h) };
	}

	//	フレームバッファのアスペクト比を取得する
	inline float GetAspectRatio() const {
		const Vector2 size = GetFramebufferSize();
		return size.x / size.y;
	}

	//	視野角の管理
	inline float GetFovY() const { return degFovY; }
	inline void SetFovY(float fovY) { 
		degFovY = fovY;
		radFovY = degFovY * Deg2Rad;
		fovScale = 1 / std::tanf(radFovY / 2);
	}
	inline float GetFovScale() const { return fovScale; }
};

#endif // !_ENGINE_H_

