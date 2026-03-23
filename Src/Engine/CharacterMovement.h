/*
 *	@file	CharacterMovement
 */

#ifndef _CHARACTERMOVEMENT_H_
#define _CHARACTERMOVEMENT_H_

#include "Component.h"
#include "GameObject.h"

 /*
  *	ゲームオブジェクトの速度と重力を扱うコンポーネント
  */
class CharacterMovement : public Component {
public:
	static constexpr float gravity = 9.81f;	//	重力加速度
	float gravityScale = 1.0f;				//	重力の係数
	Vector3 velocity = Vector3::zero;		//	速度

public:
	CharacterMovement() = default;
	virtual ~CharacterMovement() = default;

public:
	virtual void Update(float deltaTime) override {
		auto owner = GetOwner();

		if (owner->isGrounded) {
			//	下向きの加速度を０にする
			velocity.y = std::max(velocity.y, 0.0f);
		}
		else {
			//	重力加速度によって速度を更新する
			velocity.y -= gravity * gravityScale * deltaTime;
		}
		//	速度を座標に反映する
		owner->position += velocity * deltaTime;
	}
};
//	別名定義
using CharacterMovementPtr = std::shared_ptr<CharacterMovement>;

#endif // !_CHARACTERMOVEMENT_H_
