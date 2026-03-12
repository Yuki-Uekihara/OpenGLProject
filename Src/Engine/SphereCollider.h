/*
 *	@file	SphereCollider.h
 */

#ifndef _SPHERECOLLIDER_H_
#define _SPHERECOLLIDER_H_

#include "Collider.h"
#include "Collision.h"

/*
 *	球体コライダー
 */
class SphereCollider : public Collider {
public:
	Sphere sphere = { Vector3::zero, 1.0f };

public:
	SphereCollider() = default;
	virtual ~SphereCollider() = default;

	//	座標を変換する
	inline void AddPosition(const Vector3& translate) override {
		sphere.position += translate;
	}

	//	座標変換したコライダーを取得する
	inline ColliderPtr GetTransformedCollider(const Matrix4x4& transform) const override {
		//	座標返還したコピーを作成
		auto p = std::make_shared<SphereCollider>();
		p->sphere.position = Vector3(transform * Vector4(sphere.position, 1.0f));

		//	球体の拡大率を合わせる
		//	※合わせないと楕円になり、球体ではなくなってしまう
		const Vector3 scale = ExtractScale(transform);
		const float maxScale = std::max({ scale.x, scale.y, scale.z });	//	球体の拡大率はX,Y,Zのうち最大のものとする
		p->sphere.radius = sphere.radius * maxScale;
		return p;
	}

public:
	//	図形の種類
	inline Type GetType() const override { return Type::Sphere; }
	//	図形の取得
	inline const Sphere& GetShape() const { return sphere; }
};

#endif // !_SPHERECOLLIDER_H_
