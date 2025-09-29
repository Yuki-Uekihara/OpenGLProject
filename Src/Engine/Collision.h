/*
 *	@file	Collision.h
 */

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "VecMath.h"

/*
 *	�����s���E�{�b�N�X
 */
struct AABB {
	Vector3 min;
	Vector3 max;
};

//	�Փ˂��Ă��邩�ǂ���
bool Intersect(const AABB& a, const AABB b, Vector3& penetration);

#endif // !_COLLISION_H_
