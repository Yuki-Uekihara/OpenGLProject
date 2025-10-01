/*
 *	@file	GameObject.h
 */

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "VecMath.h"
#include "Component.h"
#include "Texture.h"
#include "AABBCollider.h"

#include <memory>
#include <vector>
#include <string>
#include <type_traits>

//	�O���錾
class Engine;

/*
 *	�I�u�W�F�N�g�̊��N���X 
 */
class GameObject {
	friend class Engine;	//	�t�����h�錾
private:
	Engine* engine = nullptr;						//	�G���W���̃A�h���X
	bool isDestroyed = false;						//	�폜����Ă��邩�ǂ���
	std::vector<ComponentPtr> components;			//	�R���|�[�l���g�z��
	std::vector<AABBColliderPtr> colliders;			//	�R���C�_�[�z��

public:
	std::string name;								//	���O
	Vector3 position = { 0.0f, 0.0f, 0.0f };		//	���s�ړ�
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };		//	��]�p�x
	Vector3 scale = { 1.0f, 1.0f, 1.0f };			//	�g�嗦
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//	�F
	TexturePtr texColor;							//	�J���[�e�N�X�`��
	int meshId = -1;								//	�}�`�̔ԍ�

public:
	GameObject() = default;
	virtual ~GameObject() = default;

public:
	// �ŏ���Update�̒��O�ɌĂяo�����
	virtual void Start();

	//	���t���[���Ăяo�����
	virtual void Update(float deltaTime);

	//	�Փ˂��N�����Ƃ��ɌĂяo�����
	virtual void OnCollision(
		const ComponentPtr& self,
		const ComponentPtr& other);

	//	�폜�����Ƃ��ɌĂяo�����
	virtual void OnDestroy();

public:
	//	�Q�[���I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�����
	template <class T>
	std::shared_ptr<T> AddComponent() {
		auto p = std::make_shared<T>();
		p->owner = this;

		if constexpr (std::is_base_of_v<AABBCollider, T>)
			colliders.push_back(p);
		components.push_back(p);
		p->Awake();
		return p;
	}

	//	�Q�[���I�u�W�F�N�g����R���|�[�l���g���폜����
	void RemoveComponent();

public:
	//	�Q�[���G���W�����擾
	inline Engine* GetEngine() const { return engine; }

	//	�Q�[���I�u�W�F�N�g���j������Ă��邩�ǂ���
	inline bool IsDestroyed() const { return isDestroyed; }

	//	�Q�[���I�u�W�F�N�g���G���W������폜����
	inline void Destroy() { isDestroyed = true; }
};
//	�ʖ���`
using GameObjectPtr = std::shared_ptr<GameObject>;
using GameObjectList = std::vector<GameObjectPtr>;


#endif // !_GAMEOBJECT_H_
