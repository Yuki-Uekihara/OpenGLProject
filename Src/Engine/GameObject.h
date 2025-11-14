/*
 *	@file	GameObject.h
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "VecMath.h"
#include "Component.h"
#include "Texture.h"
#include "Mesh.h"
#include "AABBCollider.h"

#include <memory>
#include <vector>
#include <string>
#include <type_traits>

//	前方宣言
class Engine;

/*
 *	描画の順番
 */
enum RenderQueue {
	RenderQueue_geometry = 2000,	//	一般的な図形
	RenderQueue_tranparent = 3000,	//	半透明な図形
	RenderQueue_overlay = 4000,		//	UI, 全画面用エフェクト
	RenderQueue_max = 5000,			//	最大値
};

/*
 *	オブジェクトの基底クラス 
 */
class GameObject {
	friend class Engine;	//	フレンド宣言
private:
	Engine* engine = nullptr;						//	エンジンのアドレス
	bool isDestroyed = false;						//	削除されているかどうか
	std::vector<ComponentPtr> components;			//	コンポーネント配列
	std::vector<AABBColliderPtr> colliders;			//	コライダー配列

public:
	std::string name;								//	名前
	Vector3 position = { 0.0f, 0.0f, 0.0f };		//	平行移動
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };		//	回転角度
	Vector3 scale = { 1.0f, 1.0f, 1.0f };			//	拡大率
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//	色
	StaticMeshPtr staticMesh;						//	表示するスタティックメッシュ
	TexturePtr texColor;							//	カラーテクスチャ
	int meshId = -1;								//	図形の番号
	int renderQueue = RenderQueue_geometry;			//	描画順

public:
	GameObject() = default;
	virtual ~GameObject() = default;

public:
	// 最初のUpdateの直前に呼び出される
	virtual void Start();

	//	毎フレーム呼び出される
	virtual void Update(float deltaTime);

	//	衝突が起きたときに呼び出される
	virtual void OnCollision(
		const ComponentPtr& self,
		const ComponentPtr& other);

	//	削除されるときに呼び出される
	virtual void OnDestroy();

public:
	//	ゲームオブジェクトにコンポーネントを追加する
	template <class T>
	std::shared_ptr<T> AddComponent() {
		auto p = std::make_shared<T>();
		p->owner = this;

		if constexpr (std::is_base_of_v<AABBCollider, T>)
			colliders.push_back(p);
		components.push_back(p);
		p->Awake();
		return p;
	}

	/*
	 *	コンポーネントを検索する
	 *	多くのコンポーネントを持つゲームオブジェクトに対して使用すると
	 *	検索に時間がかかる場合がある
	 *
	 *	@tparam	T	検索するコンポーネント
	 *	@return	T*
	 */
	template<class T>
	std::shared_ptr<T> GetComponent() {
		for (auto& c : components) {
			//	shard_ptの場合は dynamic_castではなくdynamic_pointer_castを使う
			auto p = std::dynamic_pointer_cast<T>(c);
			if (p)
				return p;	//	見つかった場合はコンポーネントを返す
		}
		return nullptr;		//	見つからなかった場合は nullptr を返す
	}

	//	ゲームオブジェクトからコンポーネントを削除する
	void RemoveComponent();

public:
	//	ゲームエンジンを取得
	inline Engine* GetEngine() const { return engine; }

	//	ゲームオブジェクトが破棄されているかどうか
	inline bool IsDestroyed() const { return isDestroyed; }

	//	ゲームオブジェクトをエンジンから削除する
	inline void Destroy() { isDestroyed = true; }
};
//	別名定義
using GameObjectPtr = std::shared_ptr<GameObject>;
using GameObjectList = std::vector<GameObjectPtr>;


#endif // !_GAMEOBJECT_H_
