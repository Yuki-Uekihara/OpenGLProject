/*
 *	@file	standard.frag
 */
#version 450

//	����
layout (location = 1) in vec2 inTexcoord;	//	�e�N�X�`�����W

//	�o��
out vec4 outColor;	//	�F

//	�v���O��������̓���
layout (binding = 0) uniform sampler2D texColor;
layout (location = 100) uniform vec4 color;		//	�F




void main() {
	vec4 c = texture(texColor, inTexcoord);

	outColor = c * color;
}