/*
 *	@file	GameObject.h
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "VecMath.h"
#include "Component.h"
#include "Texture.h"
#include "AABBCollider.h"

#include <memory>
#include <vector>
#include <string>
#include <type_traits>

//	前方宣言
class Engine;

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
	TexturePtr texColor;							//	カラーテクスチャ
	int meshId = -1;								//	図形の番号

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
