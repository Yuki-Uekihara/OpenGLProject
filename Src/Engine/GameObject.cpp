/*
 *	@file	GameObject.cpp
 */

#include "GameObject.h"
#include <algorithm>

/*
 *	�X�^�[�g�C�x���g
 */
void GameObject::Start() {
	for (auto& c : gameObjects) {
		if (!c->isStarted) {
			c->Start();
			c->isStarted = true;
		}
	}
}

/*
 *	�X�V�C�x���g
 */
void GameObject::Update(float deltaTime) {
	for (auto& c : gameObjects) {
		c->Update(deltaTime);
	}
	RemoveComponent();
}

/*
 *	�Փ˃C�x���g
 */
void GameObject::OnCollision(
	const ComponentPtr& self,
	const ComponentPtr& other) {
	for (auto& c : gameObjects) {
		c->OnCollision(self, other);
	}
}

/*
 *	�폜�C�x���g
 */
void GameObject::OnDestroy() {
	for (auto& c : gameObjects) {
		c->OnDestroy();
	}
}

/*
 *	�Q�[���I�u�W�F�N�g����R���|�[�l���g���폜����
 */ 
void GameObject::RemoveComponent() {
	//	�R���|�[�l���g���Ȃ���Ώ������Ȃ�
	if (gameObjects.empty())
		return;

	//	�j���̗\��̗L���ŃR���|�[�l���g�𕪂���
	const auto itr = std::stable_partition(
		gameObjects.begin(), gameObjects.end(),
		[](const auto& p) { return !p->IsDestroyed(); }
	);
	
	//	�폜�\��̃R���|�[�l���g��ʂ̔z��Ɉړ�
	std::vector<ComponentPtr> destroyList(
		std::move_iterator(itr),
		std::move_iterator(gameObjects.end())
	);

	//	�z�񂩂�ړ��ς݂̃R���|�[�l���g���폜
	gameObjects.erase(itr, gameObjects.end());

	//	�j���\��̃R���|�[�l���g�ɃC�x���g����
	for (auto& c : destroyList) {
		c->OnDestroy();
	}

	//	���[�J���ϐ� destroyList �̎����͂����܂�
}
