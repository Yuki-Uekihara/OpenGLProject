/*
 *	@file	Engine.h
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>

#include "GameObject.h"
#include "Scene.h"
#include "Mesh.h"

/*
 *	�Q�[���G���W��
 */
class Engine {
private:
	GLFWwindow* window = nullptr;				//	�E�B���h�E�I�u�W�F�N�g
	const std::string title = "OpenGLGame";		//	�E�B���h�E�̃^�C�g��
	float degFovY = 60;							//	��������p(�x���@)
	float radFovY = degFovY * Rad2Deg;
	float fovScale = 1 / std::tanf(radFovY / 2);	//	����p�ɂ��g�嗦�̋t��
	
	GLuint vs = 0;
	GLuint fs = 0;
	GLuint prog = 0;
	GLuint vbo = 0;		//	vertex buffer object
	GLuint ibo = 0;		//	index buffer object
	GLuint vao = 0;		//	vertex array object

	GLintptr vboSize;	//	���_�o�b�t�@�̌��݂̃T�C�Y
	GLintptr iboSize;	//	�C���f�b�N�X�o�b�t�@�̌��݂̃T�C�Y
	std::vector<DrawParam> drawParamList;	//	�`��p�����[�^�z��

	GameObjectList gameObjects;		//	�Q�[���I�u�W�F�N�g�̈ꌳ�Ǘ�
	float previousTime = 0;			//	�O��̍X�V��
	float deltaTime = 0;			//	�O��̍X�V����̌o�ߎ���

	ScenePtr scene;					//	���s���̃V�[��
	ScenePtr nextScene;				//	���̃V�[��


	GameObject camera;

	GLsizei indexCount = 0;

	//	���[���h���W�n�̃R���C�_�[��\���\����
	struct WorldCollider {
		AABBColliderPtr origin;
		AABB world;
	};
	using WorldColliderList = std::vector<WorldCollider>;

public:
	Engine() = default;
	~Engine() = default;

	int Run();

private:
	int Initialize();
	void Update();
	void Render();

	void DrawGameObject(GameObjectList::iterator begin, GameObjectList::iterator end);
	void UpdateGameObject(float deltaTime);
	void HandleGameObjectCollision();
	void HandleWorldColliderCollision(WorldColliderList* a, WorldColliderList* b);
	void RemoveGameObject();
public:
	/*
	 *	�Q�[���I�u�W�F�N�g�𐶐�����
	 *	@param	name
	 *	@param	position
	 *	@param	rotation
	 */
	template <class T>
	std::shared_ptr<T> Create(
		const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f }
	) {
		//	�I�u�W�F�N�g�̃|�C���^�𐶐�
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position = position;
		p->rotation = rotation;
		//	�ꌳ�Ǘ��ɒǉ�
		gameObjects.push_back(p);
		return p;
	}	

	//	���ׂẴQ�[���I�u�W�F�N�g���폜����
	void ClearGameObjects();

	//	���̃V�[����ݒ肷��
	template <class T>
	void SetNextScene() { nextScene = std::make_shared<T>();  }

public:
	//	�J�������擾����
	inline GameObject& GetMainCamera() { return camera; }
	inline const GameObject& GetMainCamera() const { return camera; }

	//	�L�[���͂̌��m
	inline bool GetKey(int key) const {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	//	����p�̊Ǘ�
	inline float GetFovY() const { return degFovY; }
	inline void SetFovY(float fovY) { 
		degFovY = fovY;
		radFovY = degFovY * Deg2Rad;
		fovScale = 1 / std::tanf(radFovY / 2);
	}
	inline float GetFovScale() const { return fovScale; }
};

#endif // !_ENGINE_H_

