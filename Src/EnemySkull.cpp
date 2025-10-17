/*
 *	@file	EnemySkull.cpp
 */

#include "EnemySkull.h"
#include "Engine/AABBCollider.h"
#include "Engine/Engine.h"

/*
 *	コンポーネントの初期化
 */
void EnemySkull::Awake() {
	//	モデルの設定
	auto owner = GetOwner();
	owner->meshId = MeshId_skull;
	owner->texColor = std::make_shared<Texture>("Res/MeshData/skull.tga");

	//	コライダーの設定
	auto collider = owner->AddComponent<AABBCollider>();
	collider->aabb = {
		{ -0.25f, -0.25f, -0.25f },
		{  0.25f,  0.25f,  0.25f }
	};
	collider->isTrigger = true;
}

/*
 *	コンポーネントの更新
 */
void EnemySkull::Update(float deltaTime) {
	auto owner = GetOwner();

	//	動作制御用タイマーの更新
	timer += deltaTime;

	//	上下に揺らす
	constexpr float baseY = 1.0f;		//	基準の高さ(m)
	constexpr float range = 0.1f;		//	揺らす高さ(m)
	constexpr float cycle = 5.0f;		//	揺らす感覚(秒)
	owner->position.y = baseY + std::sinf(timer * cycle) * range;

	if (getMapData) {
		const Vector3 front = { std::cosf(owner->rotation.y), 0.0f, std::sinf(owner->rotation.y) };
		const int data = getMapData(owner->position + front);

		//	前方が壁もしくは部屋の区切りかどうか
		if (data != '#' && data != '|' && data != '-') {
			//	前進
			const float speed = 1.0f;
			owner->position.x += front.x * speed * deltaTime;
			owner->position.z += front.z * speed * deltaTime;
		}
		else {
			//	左に旋回
			owner->rotation.y += 90.0f * Deg2Rad;
		}
	}
}
