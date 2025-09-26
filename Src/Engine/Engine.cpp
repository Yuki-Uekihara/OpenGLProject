/*
 *	@file	Engine.cpp
 */

#include "Engine.h"
#include <fstream>
#include <filesystem>
#include <vector>

#include "../PlayerComponent.h"

 //	�}�`�f�[�^
#include "../../Res/MeshData/crystal_mesh.h"


 /*
  *	�V�F�[�_�[�t�@�C����ǂݍ��� �{ �R���p�C��
  *	@param	type
  *	@param	filename
  *	@return	�V�F�[�_�̊Ǘ��ԍ�
  */
GLuint CompileShader(GLenum type, const char* filename) {
	//	�t�@�C�����J��
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file) {
		return 1;	//	���s
	}

	//	�t�@�C���̃T�C�Y���擾
	const size_t fileSize = std::filesystem::file_size(filename);
	//	�t�@�C���̃T�C�Y���̗̈���m��
	std::vector<char> buffer(fileSize);
	//	�ǂݍ��� + �R�s�[
	file.read(buffer.data(), fileSize);

	//	�t�@�C�������
	file.close();

	const char* source[] = { buffer.data() };
	const GLint length[] = { int(buffer.size()) };
	//	�V�F�[�_�[���쐬
	const GLuint object = glCreateShader(type);
	//	�V�F�[�_�̃\�[�X��ݒ�
	glShaderSource(object, 1, source, length);
	//	�V�F�[�_���R���p�C��
	glCompileShader(object);

	return object;
}



/*
 *	�Q�[���G���W�������s����
 *	@return	0	����
 */
int Engine::Run() {
	const int result = Initialize();
	if (result)
		return result;

	while (!glfwWindowShouldClose(window)) {
		Update();
		Render();
		RemoveGameObject();
	}

	glfwTerminate();
	return 0;
}

/*
 *	�Q�[���G���W��������������
 *	@return		0 ����
 */
int Engine::Initialize() {
	//	GLFW�̏�����
	if (glfwInit() != GLFW_TRUE)
		return 1;	//	���s

	//	�`��E�B���h�E�̍쐬
	window = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		return 1;	//	���s
	}

	//	OpenGL�R���e�L�X�g�̍쐬
	glfwMakeContextCurrent(window);

	//	OpenGL�֐��̃A�h���X���擾
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		glfwTerminate();
		return 1;	//	���s
	}

	//	�V�F�[�_�[��ǂݍ���ŃR���p�C��
	vs = CompileShader(GL_VERTEX_SHADER, "Res/standard.vert");
	fs = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");

	//	�V�F�[�_�[�������N
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);


	//	���_�f�[�^
	const Vertex vertexData[] = {
		//	+Z ��O�̖�
		{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
		{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } },

		//	-Z ���̖�
		{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f } },

		//	+Y ��̖�
		{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } },

		//	-Y ���̖�
		{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },

		//	+X �E�̖�
		{ {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f } },
		{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } },

		//	-X ���̖�
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f } },
	};

	//	���_�o�b�t�@�̊Ǘ��ԍ�
	vbo = 0;		//	vertex buffer object
	//	�o�b�t�@�̍쐬
	glCreateBuffers(1, &vbo);
	//	GPU���������m�ۂ��f�[�^���R�s�[����
	glNamedBufferStorage(vbo, sizeof(Vertex) * 10000, nullptr, 0);


	//	�C���f�b�N�X�f�[�^
	const GLushort indexData[] = {
		  0,   1,   2,    2,   3,   0,		//	+Z
		  4,   5,   6,    6,   7,   4,		//	-Z
		  8,   9,  10,   10,  11,   8,		//	+Y
		 12,  13,  14,   14,  15,  12,		//	-Y
		 16,  17,  18,   18,  19,  16,		//	+X
		 20,  21,  22,   22,  23,  20,		//	-X
	};
	//	�C���f�b�N�X���擾
	indexCount = static_cast<GLsizei>(std::size(indexData));

	//	�}�`�f�[�^�̏��
	struct MeshData {
		size_t vertexSize;			//	���_�f�[�^�̃o�C�g��
		size_t indexSize;			//	�C���f�b�N�X�f�[�^�̃o�C�g��
		const void* vertexData;		//	���_�f�[�^�̃A�h���X
		const void* indexData;		//	�C���f�b�N�X�f�[�^�̃A�h���X
	};

	const MeshData meshes[] = {
		{ sizeof(vertexData), sizeof(indexData), vertexData, indexData },
		{ sizeof(crystal_vertices), sizeof(crystal_indices), crystal_vertices, crystal_indices },

	};


	//	�C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
	ibo = 0;		//	index buffer object
	//	�o�b�t�@�̍쐬
	glCreateBuffers(1, &ibo);
	//	GPU���������m�ۂ��f�[�^���R�s�[����
	glNamedBufferStorage(ibo, sizeof(uint16_t) * 40000, nullptr, 0);

	//	�}�`�f�[�^����`��p�����[�^���쐬���A�f�[�^��GPU�������ɃR�s�[����
	drawParamList.reserve(std::size(meshes));
	for (const auto& mesh : meshes) {
		GLuint tmp[2];	//	�o�b�t�@�p�ꎞ�ϐ�
		glCreateBuffers(2, tmp);
		//	GPU�̃��������m��
		glNamedBufferStorage(tmp[0], mesh.vertexSize, mesh.vertexData, 0);
		glNamedBufferStorage(tmp[1], mesh.indexSize, mesh.indexData, 0);
		//	�f�[�^���R�s�[
		glCopyNamedBufferSubData(tmp[0], vbo, 0, vboSize, mesh.vertexSize);
		glCopyNamedBufferSubData(tmp[1], ibo, 0, iboSize, mesh.indexSize);
		glDeleteBuffers(2, tmp);	//	�ꎞ�o�b�t�@���폜

		//	�`��p�����[�^���쐬
		DrawParam param;
		param.mode = GL_TRIANGLES;
		param.count = static_cast<GLsizei>(mesh.indexSize / sizeof(uint16_t));
		param.indices = reinterpret_cast<void*>(iboSize);
		param.baseVertex = static_cast<GLint>(vboSize / sizeof(Vertex));
		drawParamList.push_back(param);		//	�`��p�����[�^��ǉ�

		//	�o�b�t�@�̃T�C�Y���X�V
		vboSize += mesh.vertexSize;
		iboSize += mesh.indexSize;
	}



	//	���_����(Vertex Attribute)�z��̊Ǘ��ԍ�
	vao = 0;		//	vertex array object
	//	VAO �𐶐�
	glCreateVertexArrays(1, &vao);
	//	OpenGL�R���e�L�X�g �Ƀo�C���h
	glBindVertexArray(vao);

	//	IBO �� OpenGL�R���e�L�X�g �� VAO �Ƀo�C���h
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//	VBO �� OpenGL�R���e�L�X�g �Ƀo�C���h
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	0�Ԗڂ̒��_����(Vertex Attribute)��L����
	glEnableVertexAttribArray(0);
	//	0�Ԗڂ̒��_������ݒ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//	1�Ԗڂ̒��_����(Vertex Attribute)��L����
	glEnableVertexAttribArray(1);
	//	1�Ԗڂ̒��_������ݒ�
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<const void*>(offsetof(Vertex, texCoord)));

	//	�ꌳ�Ǘ��z��̗e�ʂ�\��
	gameObjects.reserve(1000);

	//	�J�����𑀍삷��v���C���[�R���|�[�l���g�𐶐�
	auto player = Create<GameObject>("player", { 0.0f, 10.0f, 0.0f });
	player->AddComponent<PlayerComponent>();

	//	�J�����̐ݒ�
	camera.position = { 3, 1, 3 };
	camera.rotation.y = 3.14159265f;

	return 0;
}

/*
 *	�Q�[���G���W���̏�Ԃ��X�V����
 */
void Engine::Update() {

	//	�f���^�^�C���̌v�Z
	const float currentTime = static_cast<float>(glfwGetTime());	//	����
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	//	�f�o�b�O�p
	if (deltaTime >= 0.5f)
		deltaTime = 1.0f / 60.0f;

	//	�V�[���̐؂�ւ�
	if (nextScene) {
		if (scene)
			scene->Finalize(*this);

		nextScene->Initialize(*this);
		scene = std::move(nextScene);
	}

	//	�V�[���̍X�V
	if (scene)
		scene->Update(*this, deltaTime);


	UpdateGameObject(deltaTime);
}

/*
 *	�Q�[���G���W���̏�Ԃ�`�悷��
 */
void Engine::Render() {
	//	�o�b�N�o�b�t�@���N���A
	glClearColor(0.9f, 0.6f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	�`��Ɏg���V�F�[�_���w��
	glUseProgram(prog);

	//	�t���[���o�b�t�@(���)�̑傫�����擾
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	//	�r���[�|�[�g�̐ݒ�
	glViewport(0, 0, fbWidth, fbHeight);

	//	�A�X�y�N�g����v�Z
	const float aspectRatio =
		static_cast<float>(fbWidth) / static_cast<float>(fbHeight);

	const float degFovY = 60;	//	�����̎���p
	const float radFovY = degFovY * 3.141592f / 180.0f;
	const float scaleFov = tanf(radFovY / 2);	//	����p�ɂ��g�嗦

	//	���j�t�H�[���ϐ��Ƀf�[�^���R�s�[
	//const float timer = static_cast<float>(glfwGetTime());
	//glProgramUniform1f(prog, 0, timer);
	glProgramUniform2f(prog, 3, 1 / (aspectRatio * scaleFov), 1 / scaleFov);
	glProgramUniform3fv(prog, 4, 1, &camera.position.x);
	glProgramUniform2f(prog, 5, sinf(-camera.rotation.y), cosf(-camera.rotation.y));

	//	�[�x�e�X�g�̗L����
	glEnable(GL_DEPTH_TEST);

	//	�Q�[���I�u�W�F�N�g�̕`��
	for (const auto& obj : gameObjects) {
		//	�}�`�ԍ����ΏۊO�̏ꍇ�͕`�悵�Ȃ�
		if (obj->meshId < 0 || obj->meshId >= drawParamList.size())
			continue;

		glProgramUniform4fv(prog, 100, 1, obj->color);
		glProgramUniform3fv(prog, 0, 1, &obj->scale.x);
		glProgramUniform3fv(prog, 1, 1, &obj->position.x);
		glProgramUniform2f(prog, 2, sinf(obj->rotation.y), cosf(obj->rotation.y));

		//	�`��Ɏg���e�N�X�`�����o�C���h
		if (obj->texColor) {
			const GLuint tex = *obj->texColor;
			glBindTextures(0, 1, &tex);
		}

		//	�}�`��`��
		const DrawParam& param = drawParamList[obj->meshId];
		glDrawElementsInstancedBaseVertex(
			param.mode, param.count, GL_UNSIGNED_SHORT, param.indices, 1, param.baseVertex);
	}




	glfwSwapBuffers(window);
	glfwPollEvents();
}

/*
 *	�Q�[���I�u�W�F�N�g�̏�Ԃ��X�V����
 *	@	param	deltaTime
 */
void Engine::UpdateGameObject(float deltaTime) {
	//	�v�f�̒ǉ��ɑΉ����邽�߂� ���ʂ� for����I��
	for (int i = 0; i < gameObjects.size(); i++) {
		GameObject* pObj = gameObjects[i].get();
		if (!pObj->IsDestroyed()) {
			pObj->Start();
			pObj->Update(deltaTime);
		}
	}
}

/*
 *	�j���\��̃Q�[���I�u�W�F�N�g���폜����
 */
void Engine::RemoveGameObject() {
	//	�Q�[���I�u�W�F�N�g���Ȃ���Ώ������Ȃ�
	if (gameObjects.empty())
		return;

	//	�j���̗\��̗L���ŃQ�[���I�u�W�F�N�g�𕪂���
	const auto itr = std::stable_partition(
		gameObjects.begin(), gameObjects.end(),
		[](const auto& p) { return !p->IsDestroyed(); }
	);

	//	�폜�\��̃Q�[���I�u�W�F�N�g��ʂ̔z��Ɉړ�
	std::vector<GameObjectPtr> destroyList(
		std::move_iterator(itr),
		std::move_iterator(gameObjects.end())
	);

	//	�z�񂩂�ړ��ς݂̃Q�[���I�u�W�F�N�g���폜
	gameObjects.erase(itr, gameObjects.end());

	//	�j���\��̃Q�[���I�u�W�F�N�g�ɃC�x���g����
	for (auto& c : destroyList) {
		c->OnDestroy();
	}

	//	���[�J���ϐ� destroyList �̎����͂����܂�
}


/*
 *	�Q�[���G���W������S�ẴQ�[���I�u�W�F�N�g��j������
 */
void Engine::ClearGameObjects() {
	for (auto& obj : gameObjects) {
		obj->OnDestroy();
	}
	gameObjects.clear();
}