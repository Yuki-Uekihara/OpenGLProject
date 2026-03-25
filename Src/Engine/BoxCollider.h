/*
 *	@file	BoxCollider.h
 */

#ifndef _BOXCOLLIDER_H_
#define _BOXCOLLIDER_H_

#include "Collider.h"
#include "Collision.h"

/*
 *	Boxコライダー
 */
class BoxCollider : public Collider {
public:
	Box box = {
		Vector3::zero,
		{ Vector3::right, Vector3::up, Vector3::forward },
		Vector3::one
	};

public:
	BoxCollider() = default;
	virtual ~BoxCollider() = default;

public:
	//	座標を変更する
	inline void AddPosition(const Vector3& translate) override {
		box.position += translate;
	}

	//	座標変換したコライダーを取得する
	inline ColliderPtr GetTransformedCollider(const Matrix4x4& transform) const override {
		//	分解用の格納先を用意
		Vector3 translate;
		Vector3 scale;
		Matrix3x3 rotation;
		//	座標変換行列を分解
		Decompose(transform, translate, scale, rotation);
		
		//	座標変換したコピーを作成
		auto p = std::make_shared<BoxCollider>();
		p->box.position = Vector3(transform * Vector4(box.position, 1));
		for (int i = 0; i < 3; i++) {
			p->box.axis[i] = rotation * box.axis[i];
			p->box.scale[i] = box.scale[i] * scale[i];
		}

		return p;
	}

public:
	//	図形の種類
	inline Type GetType() const override { return Type::Box; }
	//	図形を取得する
	inline const Box& GetShape() const { return box; }
};
//	別名定義
using BoxColliderPtr = std::shared_ptr<BoxCollider>;

#endif // _BOXCOLLIDER_H_
