/*
 *	@file	standard.vert
 */
#version 450

//	“ü—Í
layout (location = 0) in vec3 inPosition;	//	’¸“_À•W


void main() {
	gl_Position = vec4(inPosition, 1);
}