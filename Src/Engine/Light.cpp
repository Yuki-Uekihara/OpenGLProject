/*
 *	@file	Light.cpp
 */

#include "Light.h"
#include "Engine.h"

Light::~Light() {
	//	ライトの解放
	GetOwner()->GetEngine()->DeallocateLight(lightIndex);
}

void Light::Awake() {
	//	ライトの取得
	if (lightIndex < 0) {
		lightIndex = GetOwner()->GetEngine()->AllocateLight();
	}
}

void Light::Update(float deltaTime) {
	//	コンポーネントの値をライトに反映する
	auto owner = GetOwner();
	auto light = owner->GetEngine()->GetLight(lightIndex);

	if (light) {
		light->position = owner->position;
		light->intensity = intensity;
		light->color = color;
		light->radius = radius;
		light->coneAngle = 0;	//	照射角度が0以下ならポイントライトとして扱う

		if (type == Type::SpotLight) {
			//	ライトの向きを計算
			Vector3 direction = Vector3::back;

			//	X軸回転
			const float sinX = std::sinf(owner->rotation.x);
			const float cosX = std::cosf(owner->rotation.x);
			direction = {
				direction.x,
				direction.y * cosX - direction.z * sinX,
				direction.y * sinX + direction.z * cosX
			};

			//	Y軸回転
			const float sinY = std::sinf(owner->rotation.y);
			const float cosY = std::cosf(owner->rotation.y);
			direction = {
				direction.x * cosY + direction.z * sinY,
				direction.y,
				direction.x * -sinY + direction.z * cosY
			};

			//	パラメータを反映
			light->direction = direction;
			light->coneAngle = coneAngle;
			light->falloffAngle = falloffAngle;
		}
	}
}
