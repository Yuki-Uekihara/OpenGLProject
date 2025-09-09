/*
 *	@file	standard.vert
 */
#version 450

//	����
layout (location = 0) in vec3 inPosition;	//	���_���W
layout (location = 1) in vec2 inTexcoord;	//	�e�N�X�`�����W

//	�o��
layout (location = 1) out vec2 outTexcoord;	//	�e�N�X�`�����W


//	�v���O��������̓���
//layout (location = 0) uniform float timer;	//	����


void main() {
	outTexcoord = inTexcoord;

	gl_Position = vec4(inPosition, 1);
}