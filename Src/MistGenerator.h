/*
 *	@file	MistGenerator.h
 */

#ifndef _MISTGENERATOR_H_
#define _MISTGENERATOR_H_

#include "Engine/Component.h"
#include "Engine/GameObject.h"
#include "Engine/Billboard.h"
#include "Engine/Random.h"
#include "Engine/Engine.h"
#include "Mist.h"

 /*
  *	霧コンポーネント
  */
class MistGenerator : public Component {
private:
	TexturePtr texMist = std::make_shared<Texture>("Res/mist.tga");
	float timer = 0.0f;

public:
	Vector2 range = Vector2::one;
	float interval = 1.0f;

public:
	MistGenerator() = default;
	virtual ~MistGenerator() = default;

public:
	virtual void Update(float deltaTime) override {
		timer += deltaTime;
		if (timer < interval) {
			return;
		}

		timer -= interval;
		auto owner = GetOwner();
		auto engine = owner->GetEngine();

		//	霧を発生させる位置をランダムに抽出
		Vector3 pos = owner->position;
		pos.x += Random::Range(-range.x, range.x);
		pos.y += Random::Range(0.15f, 0.25f);		//	調整
		pos.z += Random::Range(-range.y, range.y);

		//	霧を生成
		auto mist = engine->Create<GameObject>("mist", pos);
		mist->scale = Vector3(
			Random::Range(0.2f, 0.5f), Random::Range(0.1f, 0.2f), 1.0f
		);
		mist->meshId = MeshId_plane_xy;
		mist->texColor = texMist;
		mist->renderQueue = RenderQueue_tranparent;
		mist->AddComponent<Billboard>();
		mist->AddComponent<Mist>();
	}
};

#endif // !_MISTGENERATOR_H_
