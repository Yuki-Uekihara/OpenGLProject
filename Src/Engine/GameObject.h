/*
 *	@file	GameObject.h
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "VecMath.h"

 //	���̂̃p�����[�^
class GameObject {
public:
	Vector3 position = { 0.0f, 0.0f, 0.0f };		//	���s�ړ�
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };		//	��]�p�x
	Vector3 scale = { 1.0f, 1.0f, 1.0f };			//	�g�嗦
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//	�F
};


#endif // !_GAMEOBJECT_H_
