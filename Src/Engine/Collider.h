/*
 *	@file	Collider.h
 */

#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "Component.h"
#include "VecMath.h"
#include <memory>

/*
 *	コライダーの基底クラス
 */
class Collider : public Component {
public:
	bool isTrigger = false;		//	重複するかどうか
	bool isStatic = false;		//	動くかどうか

public:
	Collider() = default;
	virtual ~Collider() = default;

public:
	//	座標を変更する
	virtual void AddPosition(const Vector3& translate) = 0;

	//	座標変換したコライダーを取得する
	virtual std::shared_ptr<Collider> GetTransformedCollider(const Matrix4x4& transform) const = 0;

public:
	//	図形の種類
	enum class Type {
		AABB,		//	軸平行境界ボックス
		Sphere,		//	球体
		Box,		//	有向境界ボックス
	};
	virtual Type GetType() const = 0;
};
//	別名定義
using ColliderPtr = std::shared_ptr<Collider>;


#endif // !_COLLIDER_H_
