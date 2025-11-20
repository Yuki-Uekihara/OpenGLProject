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
	}
}
