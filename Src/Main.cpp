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

	//	���_�f�[�^
	const float vertexData[][3] = {
		{ -0.2f, -0.2f, 0.0f },
		{  0.2f, -0.2f, 0.0f },
		{  0.0f,  0.2f, 0.0f },

		{ -0.8f, -0.2f, 0.0f },
		{ -0.4f, -0.2f, 0.0f },
		{ -0.4f,  0.2f, 0.0f },
		{ -0.8f,  0.2f, 0.0f },
	};

	//	���_�o�b�t�@�̊Ǘ��ԍ�
	GLuint vbo = 0;		//	vertex buffer object
	//	�o�b�t�@�̍쐬
	glCreateBuffers(1, &vbo);
	//	GPU���������m�ۂ��f�[�^���R�s�[����
	glNamedBufferStorage(vbo, sizeof(vertexData), vertexData, 0);


	//	�C���f�b�N�X�f�[�^
	const GLushort indexData[] = {
		0, 1, 2,
		3, 4, 5,  5, 6, 3,
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//	���C�����[�v
	while (!glfwWindowShouldClose(window)) {
		//	�o�b�N�o�b�t�@���N���A
		glClearColor(0.9f, 0.6f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	�`��Ɏg���V�F�[�_���w��
		glUseProgram(prog);

		//	�}�`��`��
		glBindVertexArray(vao);
		glDrawElementsInstanced(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, 0, 1);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//	GLFW�̏I��
	glfwTerminate();

	return 0;
}