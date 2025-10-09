/*
 *	@file	UILayout.h
 */

#ifndef _UILAYOUT_H_
#define _UILAYOUT_H_

#include "Component.h"
#include "VecMath.h"

 /*
  *	UIを画面空間で配置するコンポーネント
  */
class UILayout : public Component {
public:
	Vector3 basePosition = Vector3::zero;	//	カメラから見た位置

public:
	UILayout() = default;
	virtual ~UILayout() = default;

public:
	void Start() override;
	void Update(float deltaTime) override;
};

#endif // !_UILAYOUT_H_
