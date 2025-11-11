/*
 *	@file	MapObject.h
 */

#ifndef _MAPOBJECT_H_
#define _MAPOBJECT_H_

#include "Engine/Component.h"
#include <vector>

/*
 *	マップに配置するオブジェクト
 */
class MapObject : public Component {
private:
	//	遠隔操作するマップオブジェクトの配列
	std::vector<std::shared_ptr<MapObject>> remoteObjects;

public:
	MapObject() = default;
	virtual ~MapObject() = default;

public:
	enum class ActionResult {
		noRemoteAction,			//	リモートアクションを実行しない
		executeRemoteAction,	//	リモートアクションを実行する
	};

public:
	/*
	 *	自分がクリックされたときに実行される仮想関数
	 */
	virtual ActionResult Action() {
		return ActionResult::executeRemoteAction;
	}

	/*
	 *	関連付けされたオブジェクトがクリックされたときに実行される仮想関数
	 *	@param	linkedObject
	 */
	virtual void RemoteAction(MapObject* linkedObject) {

	}

public:
	/*
	 *	クリックされたときに呼ぶ関数
	 */
	void OnClick() {
		if (Action() == ActionResult::executeRemoteAction) {
			InvokeRemoteAction();
		}
	}

	/*
	 *	遠隔操作を実行する
	 */
	void InvokeRemoteAction() {
		for (auto& obj : remoteObjects) {
			obj->RemoteAction(this);
		}
	}

	/*
	 *	遠隔操作するマップオブジェクトを追加する
	 *	@param	object
	 */
	void AddRemoteObject(const std::shared_ptr<MapObject>& object) {
		remoteObjects.push_back(object);
	}
};
//	別名定義
using MapObjectPtr = std::shared_ptr<MapObject>;

#endif // !_MAPOBJECT_H_
