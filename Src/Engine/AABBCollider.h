/*
 *	@file	AABBCollider.h
 */

#ifndef _AABBCOLLIDER_H_
#define _AABBCOLLIDER_H_

#include "Collider.h"
#include "Collision.h"

/*
 *	AABBコライダー
 */
class AABBCollider : public Collider {
public:
	AABB aabb = {
		{ -1, -1, -1 },
		{  1,  1,  1 },
	};					//	衝突判定

public:
	AABBCollider() = default;
	virtual ~AABBCollider() = default;

public:
	//	座標を変換する
	inline void AddPosition(const Vector3& translate) override {
		aabb.min += translate;
		aabb.max += translate;
	}

	//	座標変換したコライダーを取得する
	inline ColliderPtr GetTransformedCollider(const Matrix4x4& transform) const override {
		//	※AABBなので回転は考慮しない、回転角を90度単位で回さなければならない
		//	座標変換したコピーを作成
		auto p = std::make_shared<AABBCollider>();
		p->aabb.min = Vector3(transform * Vector4(aabb.min, 1));
		p->aabb.max = Vector3(transform * Vector4(aabb.max, 1));

		//	minのほうが大きかったら入れ替える
		for (int i = 0; i < 3; i++) {
			if (p->aabb.min[i] > p->aabb.max[i]) {
				const float tmp = p->aabb.min[i];
				p->aabb.min[i] = p->aabb.max[i];
				p->aabb.max[i] = tmp;
			}
		}
		return p;
	}

public:
	//	図形の種類
	inline Type GetType() const override { return Type::AABB; }
	//	図形の取得
	inline const AABB& GetShape() const { return aabb; }
};
//	別名定義
using AABBColliderPtr = std::shared_ptr<AABBCollider>;

#endif // !_AABBCOLLIDER_H_
