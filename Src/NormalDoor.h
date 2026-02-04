/*
 *	@file	NormalDoor.h 
 */

#ifndef NORMALDOOR_H_
#define NORMALDOOR_H_

#include "MapObject.h"
#include "Engine/GameObject.h"
#include "Engine/AABBCollider.h"

/*
 *	クリックで開閉するドア
 */
class NormalDoor : public MapObject {
private:
	//	ドアの開閉状態
	enum class State {
		Closed,		//	閉じている
		Opening,	//	開いている途中
		Open,		//	開いている
		Closing,	//	閉じている途中
	};
	State state = State::Closed;
	GameObjectPtr door;
	AABBColliderPtr collider;

public:
	NormalDoor() = default;
	virtual ~NormalDoor() = default;

public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;

	virtual ActionResult Action() override;
};


#endif // !NORMALDOOR_H_
