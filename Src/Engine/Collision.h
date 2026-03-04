/*
 *	@file	Collision.h
 */

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "VecMath.h"

/*
 *	軸平行境界ボックス
 */
struct AABB {
	Vector3 min;
	Vector3 max;
};

/*
 *	球体
 */
struct Sphere {
	Vector3 position;	//	中心点
	float radius;		//	半径
};

/*
 *	光線
 */
struct Ray {
	Vector3 origin;			//	始点
	Vector3 direction;		//	方向
};

//	衝突しているかどうか
bool Intersect(const AABB& a, const AABB& b, Vector3& penetration);
bool Intersect(const Sphere& a, const Sphere& b, Vector3& penetration);
bool Intersect(const AABB& aabb, const Sphere& sphere, Vector3& penetration);

bool Intersect(const AABB& aabb, const Ray& ray, float& distance);
bool Intersect(const Sphere& sphere, const Ray& ray, float& distance);

#endif // !_COLLISION_H_
