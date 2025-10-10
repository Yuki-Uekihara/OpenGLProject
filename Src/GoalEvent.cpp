/*
 *	@file	GoalEvent.cpp
 */

#include "GoalEvent.h"
#include "Engine/Engine.h"
#include "Engine/UILayout.h"

/*
 *	�S�[�����o�̏Փ˃C�x���g
 *	@param self
 *	@param other
 */
void GoalEvent::OnCollision(const ComponentPtr& self, const ComponentPtr& other) {
	//	���Ƀv���C���[�ƏՓ˂��Ă����珈�����Ȃ�
	if (triggerd || other->GetOwner()->name != "player")
		return;

	triggerd = true;	//	�Փˍς݂ɂ���

	//	���b�Z�[�W�I�u�W�F�N�g�𐶐�
	Engine* engine = GetOwner()->GetEngine();
	engine->CreateUIObject<UILayout>("Res/goal_text.tga", { 0.0f, 0.0f }, 0.1f);
}
