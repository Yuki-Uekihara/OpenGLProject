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
layout (location = 0) uniform vec3 scale;		//	�g�嗦
layout (location = 1) uniform vec3 position;	//	���s�ړ�
layout (location = 2) uniform vec2 sinCosY;		//	Y����]

void main() {
	outTexcoord = inTexcoord;

	//	�g�k
	vec3 pos = inPosition * scale;

	//	��](Y��)
	float sinY = sinCosY.x;
	float cosY = sinCosY.y;
	gl_Position.x = pos.x * cosY + pos.z * sinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -sinY + pos.z * cosY;

	//	���s�ړ�
	gl_Position.xyz += position;	//	�X�E�B�Y�����O(swizzling)
	
	//	���ߖ@��L���ɂ���
	gl_Position.zw = -gl_Position.zz;
}