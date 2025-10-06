/*
 *	@file	Component.h
 */

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <memory>

//	前方宣言
class GameObject;

/*
 *	コンポーネントの基底クラス
 */
class Component {
	friend class GameObject;	//	フレンド宣言

private:
	GameObject* owner;			//	このコンポーネントの所有者
	bool isStarted = false;		//	Startが実行されたらtrue
	bool isDestroyed = false;	//	Destroyが実行されたらtrue

public:
	Component() = default;
	virtual ~Component() = default;

public:
	//	ゲームオブジェクトに追加されたときに呼び出される
	virtual void Awake() {}

	// 最初のUpdateの直前に呼び出される
	virtual void Start() {}

	//	毎フレーム呼び出される
	virtual void Update(float deltaTime) {}

	//	衝突が起きたときに呼び出される
	virtual void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {}

	//	削除されるときに呼び出される
	virtual void OnDestroy() {}

public:
	//	コンポーネントの所有者を取得
	inline GameObject* GetOwner() const { return owner; }

	//	コンポーネントが破棄されているかどうか
	inline bool IsDestroyed() const { return isDestroyed; }

	//	コンポーネントをゲームオブジェクトから削除する
	inline void Destroy() { isDestroyed = true; }
};

//	別名定義
using ComponentPtr = std::shared_ptr<Component>;

#endif // !_COMPONENT_H_
