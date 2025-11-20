/*
 *	@file	Light.h
 */

#ifndef _LIGHT_H
#define _LIGHT_H

#include "Component.h"
#include "VecMath.h"

/*
 *	ライトコンポーネント
 */
class Light : public Component {
private:
	int lightIndex = -1;					//	管理番号

public:
	Vector3 color = { 1.0f, 1.0f, 1.0f };	//	色
	float intensity = 1.0f;					//	明るさ
	float radius = 1.0f;					//	半径

public:
	Light() = default;
	virtual ~Light();

public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
};

#endif // !_LIGHT_H
