/*
 *	@file	standard.vert
 */
#version 450

//	����
layout (location = 0) in vec3 inPosition;	//	���_���W

//	�o��
layout (location = 0) out vec4 outColor;	//	���_�F


//	�v���O��������̓���
layout (location = 0) uniform float timer;	//	����


void main() {

	const vec4 colors[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },		//	��
		{ 0.0f, 1.0f, 0.0f, 1.0f },		//	��
		{ 0.0f, 0.0f, 1.0f, 1.0f },		//	��
	};

	int i = gl_VertexID + int(timer);

	//outColor = colors[gl_VertexID % 3];
	outColor = colors[i % 3];

	gl_Position = vec4(inPosition, 1);
}