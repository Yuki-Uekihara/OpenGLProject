/*
 *	@file	EnemySkull.h 
 */

#ifndef _ENEMYSKULL_H_
#define _ENEMYSKULL_H_

#include "Engine/Component.h"
#include <functional>
#include "Engine/VecMath.h"

/*
 *	敵 
 */
class EnemySkull : public Component {
private:
	float timer = 0.0f;		//	制御用

public:
	//	マップデータを参照する関数オブジェクト
	std::function<int(const Vector3&)> getMapData;

public:
	EnemySkull() = default;
	virtual ~EnemySkull() = default;

public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
};

#endif // !_ENEMYSKULL_H_
