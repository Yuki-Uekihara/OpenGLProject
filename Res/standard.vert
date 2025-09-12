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
layout (location = 3) uniform float aspectRatio;	//	�A�X�y�N�g��
layout (location = 4) uniform vec3 cameraPosition;	//	�J�����̕��s�ړ�
layout (location = 5) uniform vec2 cameraSinCosY;	//	�J������Y����]


void main() {
	outTexcoord = inTexcoord;

	//	���[�J�����W�n���烏�[���h���W�n�ɕϊ�
	vec3 pos = inPosition * scale;

	float sinY = sinCosY.x;
	float cosY = sinCosY.y;
	gl_Position.x = pos.x * cosY + pos.z * sinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -sinY + pos.z * cosY;

	gl_Position.xyz += position;	//	�X�E�B�Y�����O(swizzling)
	
	//	���[���h���W�n����r���[���W�n�ɕϊ�
	pos = gl_Position.xyz - cameraPosition;
	float cameraSinY = cameraSinCosY.x;
	float cameraCosY = cameraSinCosY.y;
	gl_Position.x = pos.x * cameraCosY + pos.z * cameraSinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -cameraSinY + pos.z * cameraCosY;

	//	�r���[���W�n����N���b�v���W�n�ɕϊ�
	gl_Position.x /= aspectRatio;

	//	�[�x�l�̌v�Z���ʂ� -1 ~ +1 �ɂȂ�悤��A, B���v�Z
	const float near = 0.5f;
	const float far = 1000.0f;
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);

	//	���ߖ@��L���ɂ���
	gl_Position.w = -gl_Position.z;
	gl_Position.z = -gl_Position.z * B + A;	//	�[�x�l�̕␳
}