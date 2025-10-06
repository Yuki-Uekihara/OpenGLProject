/*
 *	@file	GoalEvent.h
 */

#ifndef _GOALEVENT_H_
#define _GOALEVENT_H_

#include "Engine/Component.h"

/*
 *	ゴール演出用コンポーネント
 */
class GoalEvent : public Component {
private:
	bool triggerd = false;	//	ゴール判定が起動したらtrue

public:
	GoalEvent() = default;
	virtual ~GoalEvent() = default;

private:
	virtual void OnCollision(
		const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;
};

#endif // !_GOALEVENT_H_
