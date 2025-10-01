/*
 *	@file	GameObject.cpp
 */

#include "GameObject.h"
#include <algorithm>

/*
 *	�X�^�[�g�C�x���g
 */
void GameObject::Start() {
	for (auto& c : components) {
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
	for (auto& c : components) {
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
	for (auto& c : components) {
		c->OnCollision(self, other);
	}
}

/*
 *	�폜�C�x���g
 */
void GameObject::OnDestroy() {
	for (auto& c : components) {
		c->OnDestroy();
	}
}

/*
 *	�Q�[���I�u�W�F�N�g����R���|�[�l���g���폜����
 */ 
void GameObject::RemoveComponent() {
	//	�R���|�[�l���g���Ȃ���Ώ������Ȃ�
	if (components.empty())
		return;

	//	�j���̗\��̗L���ŃR���|�[�l���g�𕪂���
	const auto itr = std::stable_partition(
		components.begin(), components.end(),
		[](const auto& p) { return !p->IsDestroyed(); }
	);
	
	//	�폜�\��̃R���|�[�l���g��ʂ̔z��Ɉړ�
	std::vector<ComponentPtr> destroyList(
		std::move_iterator(itr),
		std::move_iterator(components.end())
	);

	//	�z�񂩂�ړ��ς݂̃R���|�[�l���g���폜
	components.erase(itr, components.end());

	//	colliders���폜
	const auto itr2 = std::remove_if(
		colliders.begin(), colliders.end(),
		[](const auto& p) { return !p->IsDestroyed(); }
	);
	colliders.erase(itr2, colliders.end());

	//	�j���\��̃R���|�[�l���g�ɃC�x���g����
	for (auto& c : destroyList) {
		c->OnDestroy();
	}

	//	���[�J���ϐ� destroyList �̎����͂����܂�
}
