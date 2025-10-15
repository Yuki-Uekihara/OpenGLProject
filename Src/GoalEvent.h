/*
 *	@file	GoalEvent.h
 */

#ifndef _GOALEVENT_H_
#define _GOALEVENT_H_

#include "Engine/Component.h"
#include "Engine/GameObject.h"

/*
 *	�S�[�����o�p�R���|�[�l���g
 */
class GoalEvent : public Component {
private:
	bool triggerd = false;	//	�S�[�����肪�N��������true

	GameObjectPtr fadeObject;	//	�t�F�[�h�p�I�u�W�F�N�g
	float fadeTimer;			//	�t�F�[�h���̃^�C�}�[

public:
	GoalEvent() = default;
	virtual ~GoalEvent() = default;

public:
	virtual void Update(float deltaTime) override;

private:
	virtual void OnCollision(
		const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;
};

#endif // !_GOALEVENT_H_
