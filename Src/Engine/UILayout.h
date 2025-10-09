/*
 *	@file	UILayout.h
 */

#ifndef _UILAYOUT_H_
#define _UILAYOUT_H_

#include "Component.h"
#include "VecMath.h"

 /*
  *	UI����ʋ�ԂŔz�u����R���|�[�l���g
  */
class UILayout : public Component {
public:
	Vector3 basePosition = Vector3::zero;	//	�J�������猩���ʒu

public:
	UILayout() = default;
	virtual ~UILayout() = default;

public:
	void Start() override;
	void Update(float deltaTime) override;
};

#endif // !_UILAYOUT_H_
