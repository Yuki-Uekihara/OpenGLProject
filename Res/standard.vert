/*
 *	@file	standard.vert
 */
#version 450

//	����
layout (location = 0) in vec3 inPosition;	//	���_���W


void main() {
	gl_Position = vec4(inPosition, 1);
}