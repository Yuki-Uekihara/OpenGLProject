/*
 *	@file	FluorescentLight.h
 */

#ifndef _FLUORESCENTLIGHT_H_
#define _FLUORESCENTLIGHT_H_

#include "Engine/Component.h"
#include "Engine/Engine.h"
#include "Engine/Billboard.h"
#include "Engine/Light.h"

/*
 *	蛍光灯コンポーネント
 */
class FluorescentLight : public Component {
public:
	FluorescentLight() = default;
	virtual ~FluorescentLight() = default;

public:
	//	ゲームオブジェクトに追加されたときに呼び出される
	virtual void Awake() override {
		//　蛍光灯モデルを設定
		auto owner = GetOwner();
		auto engine = owner->GetEngine();
		owner->staticMesh = engine->GetStaticMesh("Res/MeshData/HorrorHospital/Lamp.obj");

		//	ぼんやりとした光
		auto glare = engine->Create<GameObject>("glare", owner->position);
		glare->position.y -= 0.25f;
		glare->renderQueue = RenderQueue_tranparent;
		glare->meshId = MeshId_plane_xy;
		glare->texColor = std::make_shared<Texture>("Res/glare.tga");
		glare->AddComponent<Billboard>();

		auto light = owner->AddComponent<Light>();
		light->color = { 0.9f, 1.0f, 0.9f };
		light->intensity = 20.0f;
		light->radius = 4.0f;
		light->type = Light::Type::SpotLight;
	}
};

#endif // !_FLUORESCENTLIGHT_H_
