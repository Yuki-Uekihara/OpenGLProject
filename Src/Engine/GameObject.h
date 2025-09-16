/*
 *	@file	GameObject.h
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "VecMath.h"

 //	物体のパラメータ
class GameObject {
public:
	Vector3 position = { 0.0f, 0.0f, 0.0f };		//	平行移動
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };		//	回転角度
	Vector3 scale = { 1.0f, 1.0f, 1.0f };			//	拡大率
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//	色
};


#endif // !_GAMEOBJECT_H_
