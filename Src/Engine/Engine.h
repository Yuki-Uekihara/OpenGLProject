/*
 *	@file	Engine.h
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <utility>	//	std::pair

#include "GameObject.h"
#include "Scene.h"
#include "Mesh.h"


//	���b�V���ԍ�
//	Initialize�֐��ɂ��� meshes �z��Ə��Ԃ����킹��
//	�z��ɕύX���������ꍇ�͂�������ύX����
enum MeshId {
	MeshId_box,
	MeshId_crystal,
	MeshId_wall,
	MeshId_plane_xy
};


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
	 *	@tparam	�Q�[���I�u�W�F�N�g�܂��͂��̔h���N���X
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

	/*
	 *	UI�I�u�W�F�N�g���쐬����
	 *	@tparam	�Q�[���I�u�W�F�N�g�Ɋ��蓖�Ă�UILayout�܂��͂��̔h���N���X
	 *	@param	filename
	 *	@param	position
	 *	@param	scale
	 */
	template <class T>
	std::pair<GameObjectPtr, std::shared_ptr<T>> CreateUIObject(
		const char* filename, const Vector2& position, float scale
	) {
		//	���b�Z�[�W�I�u�W�F�N�g�𐶐�
		auto obj = Create<GameObject>(filename, { position.x, position.y, 0 });
		obj->texColor = std::make_shared<Texture>(filename);
		obj->meshId = MeshId_plane_xy;
		obj->renderQueue = RenderQueue_overlay;

		//	�摜�T�C�Y�ɉ����Ċg�嗦�𒲐�
		const float aspectRatio = obj->texColor->GetAspectRatio();
		obj->scale = { scale * aspectRatio, scale, 1.0f };

		//	�R���|�[�l���g�̒ǉ�
		auto component = obj->AddComponent<T>();

		return { obj, component };
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

	//	�}�E�X�J�[�\���̍��W���擾����
	inline Vector2 GetMousePosition() const {
		//	�X�N���[�����W�n�̃J�[�\�����W���擾
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		//	�^�����킹��
		const Vector2 pos = { static_cast<float>(x), static_cast<float>(y) };

		//	UILayer�̍��W�n�ƕ����邽�߂� �X�N���[�����W�n����J�������W�n�ɕϊ�
		//	�J�������W�n�̃}�E�X���W.x =
		//		((�X�N���[�����W�n�̃}�E�X���W.x / ��ʃT�C�Y.x) * 2 - 1) * �A�X�y�N�g��
		//	
		//	�J�������W�n�̃}�E�X���W.y =
		//		((�X�N���[�����W�n�̃}�E�X���W.y / ��ʃT�C�Y.y) * 2 - 1) * -1

		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		const Vector2 framebufferSize = { static_cast<float>(w), static_cast<float>(h) };
		const float aspectRatio = framebufferSize.x / framebufferSize.y;

		return { (pos.x / framebufferSize.x * 2 - 1) * aspectRatio, (pos.y / framebufferSize.y * 2 - 1) * -1 };
	}

	//	�}�E�X�̃{�^���̏�Ԃ��擾����
	inline bool GetMouseButton(int button) const {
		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	}

	//	�t���[���o�b�t�@�̃T�C�Y���擾����
	inline Vector2 GetFramebufferSize() const {
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		return { static_cast<float>(w), static_cast<float>(h) };
	}

	//	�t���[���o�b�t�@�̃A�X�y�N�g����擾����
	inline float GetAspectRatio() const {
		const Vector2 size = GetFramebufferSize();
		return size.x / size.y;
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

