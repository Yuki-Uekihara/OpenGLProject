/*
 *	@file	Component.h
 */

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <memory>

//	�O���錾
class GameObject;

/*
 *	�R���|�[�l���g�̊��N���X
 */
class Component {
	friend class GameObject;	//	�t�����h�錾

private:
	GameObject* owner;			//	���̃R���|�[�l���g�̏��L��
	bool isStarted = false;		//	Start�����s���ꂽ��true
	bool isDestroyed = false;	//	Destroy�����s���ꂽ��true

public:
	Component() = default;
	virtual ~Component() = default;

public:
	//	�Q�[���I�u�W�F�N�g�ɒǉ����ꂽ�Ƃ��ɌĂяo�����
	virtual void Awake() {}

	// �ŏ���Update�̒��O�ɌĂяo�����
	virtual void Start() {}

	//	���t���[���Ăяo�����
	virtual void Update(float deltaTime) {}

	//	�Փ˂��N�����Ƃ��ɌĂяo�����
	virtual void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {}

	//	�폜�����Ƃ��ɌĂяo�����
	virtual void OnDestroy() {}

public:
	//	�R���|�[�l���g�̏��L�҂��擾
	inline GameObject* GetOwner() const { return owner; }

	//	�R���|�[�l���g���j������Ă��邩�ǂ���
	inline bool IsDestroyed() const { return isDestroyed; }

	//	�R���|�[�l���g���Q�[���I�u�W�F�N�g����폜����
	inline void Destroy() { isDestroyed = true; }
};

//	�ʖ���`
using ComponentPtr = std::shared_ptr<Component>;

#endif // !_COMPONENT_H_
