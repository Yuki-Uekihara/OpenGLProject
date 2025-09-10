/*
 *	@file	Main.cpp
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>

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
 *	�e�N�X�`����ǂݍ���
 *	@param	filename
 *	@return	�e�N�X�`���̊Ǘ��ԍ�
 */
GLuint LoadTexture(const char* filename) {
	//	�t�@�C�����J��
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file) {
		return 1;	//	���s
	}

	//	�t�@�C���̃T�C�Y���擾
	const size_t fileSize = std::filesystem::file_size(filename);
	//	�t�@�C���̃T�C�Y���̗̈���m��
	std::vector<uint8_t> buffer(fileSize);
	//	�ǂݍ��� + �R�s�[
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

	//	�t�@�C�������
	file.close();

	//	TGA�w�b�_��������擾
	const size_t tgaHeaderSize = 18;	//	�w�b�_�[���̃o�C�g��
	const int width = buffer[12] + buffer[13] * 256;
	const int height = buffer[14] + buffer[15] * 256;

	//	�e�N�X�`�����쐬
	GLuint object = 0;		//	�Ǘ��ԍ�
	glCreateTextures(GL_TEXTURE_2D, 1, &object);
	glTextureStorage2D(object, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(
		object,
		0,
		0, 0,
		width, height,
		GL_BGRA,
		GL_UNSIGNED_BYTE,
		buffer.data() + tgaHeaderSize
	);

	return object;
}


/*
 *	�G���g���[�|�C���g
 */
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {

	//	GLFW�̏�����
	if (glfwInit() != GLFW_TRUE)
		return 1;	//	���s

	//	�`��E�B���h�E�̍쐬
	GLFWwindow* window = nullptr;				//	�E�B���h�E�I�u�W�F�N�g
	const std::string title = "OpenGLGame";		//	�E�B���h�E�̃^�C�g��
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
	const GLuint vs = CompileShader(GL_VERTEX_SHADER, "Res/standard.vert");
	const GLuint fs = CompileShader(GL_FRAGMENT_SHADER, "Res/standard.frag");

	//	�V�F�[�_�[�������N
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	//	Vector2
	struct Vector2 { float x, y; };
	//	Vector3
	struct Vector3 { float x, y, z; };
	//	���_���
	struct Vertex {
		Vector3 position;		//	���_���W
		Vector2 texCoord;		//	�e�N�X�`�����W
	};

	//	���_�f�[�^
	const Vertex vertexData[] = {
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } },
	};

	//	���_�o�b�t�@�̊Ǘ��ԍ�
	GLuint vbo = 0;		//	vertex buffer object
	//	�o�b�t�@�̍쐬
	glCreateBuffers(1, &vbo);
	//	GPU���������m�ۂ��f�[�^���R�s�[����
	glNamedBufferStorage(vbo, sizeof(vertexData), vertexData, 0);


	//	�C���f�b�N�X�f�[�^
	const GLushort indexData[] = {
		0, 1, 2, 2, 3, 0
	};

	//	�C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
	GLuint ibo = 0;		//	index buffer object
	//	�o�b�t�@�̍쐬
	glCreateBuffers(1, &ibo);
	//	GPU���������m�ۂ��f�[�^���R�s�[����
	glNamedBufferStorage(ibo, sizeof(indexData), indexData, 0);


	//	���_����(Vertex Attribute)�z��̊Ǘ��ԍ�
	GLuint vao = 0;		//	vertex array object
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

	//	�e�N�X�`�����쐬
	GLuint tex = LoadTexture("Res/box.tga");

	//	���̂̃p�����[�^
	class GameObject {
	public:
		Vector3 position = { 0.0f, 0.0f, 0.0f };		//	���s�ړ�
		Vector3 rotation = { 0.0f, 0.0f, 0.0f };		//	��]�p�x
		Vector3 scale = { 1.0f, 1.0f, 1.0f };			//	�g�嗦
		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//	�F
	};

	GameObject box0;
	box0.scale = { 0.1f, 0.1f, 0.1f };
	box0.position = { -0.6f, -0.6f, -1.0f };


	GameObject box1;
	box1.color[1] = 0.5f;
	box1.scale = { 0.2f, 0.2f, 0.2f };
	box1.position = { 0.0f, 0.0f, -0.8f };

	//	���C�����[�v
	while (!glfwWindowShouldClose(window)) {
		//	�Ă���
		box0.rotation.y += 0.1f;


		//	�o�b�N�o�b�t�@���N���A
		glClearColor(0.9f, 0.6f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	�`��Ɏg���V�F�[�_���w��
		glUseProgram(prog);

		//	���j�t�H�[���ϐ��Ƀf�[�^���R�s�[
		//const float timer = static_cast<float>(glfwGetTime());
		//glProgramUniform1f(prog, 0, timer);
		glProgramUniform4fv(prog, 100, 1, box0.color);
		glProgramUniform3fv(prog, 0, 1, &box0.scale.x);
		glProgramUniform3fv(prog, 1, 1, &box0.position.x);
		glProgramUniform2f(prog, 2, sinf(box0.rotation.y), cosf(box0.rotation.y));


		//	�`��Ɏg���e�N�X�`�����o�C���h
		glBindTextures(0, 1, &tex);

		//	�}�`��`��
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 1);

		glProgramUniform4fv(prog, 100, 1, box1.color);
		glProgramUniform3fv(prog, 0, 1, &box1.scale.x);
		glProgramUniform3fv(prog, 1, 1, &box1.position.x);
		glProgramUniform2f(prog, 2, sinf(box1.rotation.y), cosf(box1.rotation.y));

		//	�}�`��`��
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//	GLFW�̏I��
	glfwTerminate();

	return 0;
}