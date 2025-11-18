/*
 *	@file	Mist.h
 */

#ifndef _MIST_H_
#define _MIST_H_

#include "Engine/Component.h"

/*
 *	霧コンポーネント
 */
class Mist : public Component {
	friend class MistGenerator;

private:
	float timer = 0.0f;
	float lifespan = 7.0f;

public:
	Mist() = default;
	virtual ~Mist() = default;

public:
	virtual void Update(float deltaTime) override {
		//	タイマーが寿命を迎えたら自身を削除する
		timer += deltaTime;

		auto owner = GetOwner();
		owner->color[3] = std::sinf(timer * Pi / lifespan);

		if (timer >= lifespan) {
			owner->Destroy();
		}
	}
};

#endif // !_MIST_H_
