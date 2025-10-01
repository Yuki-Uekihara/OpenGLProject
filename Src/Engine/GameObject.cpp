/*
 *	@file	GameObject.cpp
 */

#include "GameObject.h"
#include <algorithm>

/*
 *	スタートイベント
 */
void GameObject::Start() {
	for (auto& c : components) {
		if (!c->isStarted) {
			c->Start();
			c->isStarted = true;
		}
	}
}

/*
 *	更新イベント
 */
void GameObject::Update(float deltaTime) {
	for (auto& c : components) {
		c->Update(deltaTime);
	}
	RemoveComponent();
}

/*
 *	衝突イベント
 */
void GameObject::OnCollision(
	const ComponentPtr& self,
	const ComponentPtr& other) {
	for (auto& c : components) {
		c->OnCollision(self, other);
	}
}

/*
 *	削除イベント
 */
void GameObject::OnDestroy() {
	for (auto& c : components) {
		c->OnDestroy();
	}
}

/*
 *	ゲームオブジェクトからコンポーネントを削除する
 */ 
void GameObject::RemoveComponent() {
	//	コンポーネントがなければ処理しない
	if (components.empty())
		return;

	//	破棄の予定の有無でコンポーネントを分ける
	const auto itr = std::stable_partition(
		components.begin(), components.end(),
		[](const auto& p) { return !p->IsDestroyed(); }
	);
	
	//	削除予定のコンポーネントを別の配列に移動
	std::vector<ComponentPtr> destroyList(
		std::move_iterator(itr),
		std::move_iterator(components.end())
	);

	//	配列から移動済みのコンポーネントを削除
	components.erase(itr, components.end());

	//	collidersも削除
	const auto itr2 = std::remove_if(
		colliders.begin(), colliders.end(),
		[](const auto& p) { return !p->IsDestroyed(); }
	);
	colliders.erase(itr2, colliders.end());

	//	破棄予定のコンポーネントにイベント発火
	for (auto& c : destroyList) {
		c->OnDestroy();
	}

	//	ローカル変数 destroyList の寿命はここまで
}
