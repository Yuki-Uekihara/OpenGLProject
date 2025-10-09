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
	auto obj = engine->Create<GameObject>("goal text", Vector3::back);
	
	obj->texColor = std::make_shared<Texture>("Res/goal_text.tga");
	obj->meshId = 2;
	obj->renderQueue = RenderQueue_overlay;
	//	�R���|�[�l���g�̒ǉ�
	obj->AddComponent<UILayout>();
}
