/*
 *	@file	Billboard.h
 */

#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "Component.h"
#include "Engine.h"

/*
 *	ビルボード化コンポーネント 
 */
class Billboard : public Component {
public:
	Billboard() = default;
	virtual ~Billboard() = default;

	virtual void Update(float deltaTime) override {
		//	常にカメラの方を向く
		GameObject* owner = GetOwner();
		const Engine* engine = owner->GetEngine();
		const GameObject& camera = engine->GetMainCamera();
		owner->rotation.y = camera.rotation.y;
	}

};

#endif // !_BILLBOARD_H_
