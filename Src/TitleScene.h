/*
 *	@file	TitleScene.h
 */

#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_

#include "Engine/Scene.h"
#include "Engine/Engine.h"
#include <vector>


/*
 *	ƒ^ƒCƒgƒ‹‰æ–Ê
 */
class TitleScene : public Scene {
public:
	TitleScene() = default;
	virtual ~TitleScene() = default;

public:
	virtual bool Initialize(Engine& engine) override;
	virtual void Finalize(Engine& engine) override;
};

#endif // !_TITLESCENE_H_
