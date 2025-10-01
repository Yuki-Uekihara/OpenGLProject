/*
 *	@file	AABBCollider.h
 */

#ifndef _AABBCOLLIDER_H_
#define _AABBCOLLIDER_H_

#include "Component.h"
#include "Collision.h"

/*
 *	AABB�R���C�_�[
 */
class AABBCollider : public Component {
public:
	bool isTrigger = false;		//	�d�����邩�ǂ���
	bool isStatic = false;		//	�������ǂ���
	AABB aabb;					//	�Փ˔���

public:
	AABBCollider() = default;
	virtual ~AABBCollider() = default;
};
//	�ʖ���`
using AABBColliderPtr = std::shared_ptr<AABBCollider>;

#endif // !_AABBCOLLIDER_H_
