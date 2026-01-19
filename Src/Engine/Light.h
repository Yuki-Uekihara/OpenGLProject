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
	//	種類
	enum class Type {
		PointLight,		//	点光源
		SpotLight,		//	スポット光源
	};
	Type type = Type::PointLight;

	Vector3 color = { 1.0f, 1.0f, 1.0f };	//	色
	float intensity = 1.0f;					//	明るさ
	float radius = 1.0f;					//	半径

	float coneAngle = 30 * Deg2Rad;			//	スポットライトが照らす角度
	float falloffAngle = 20 * Deg2Rad;		//	スポットライトの減衰開始角度

public:
	Light() = default;
	virtual ~Light();

public:
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
};

#endif // !_LIGHT_H
