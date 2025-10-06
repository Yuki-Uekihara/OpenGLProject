/*
 *	@file	GoalEvent.cpp
 */

#include "GoalEvent.h"
#include "Engine/Engine.h"

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
	auto obj = engine->Create<GameObject>("goal text");
	obj->position = GetOwner()->position;
	obj->texColor = std::make_shared<Texture>("Res/goal_text.tga");
	obj->meshId = 2;
}
