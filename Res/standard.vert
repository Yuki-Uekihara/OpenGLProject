/*
 *	@file	standard.vert
 */
#version 450

//	入力
layout (location = 0) in vec3 inPosition;	//	頂点座標
layout (location = 1) in vec2 inTexcoord;	//	テクスチャ座標

//	出力
layout (location = 1) out vec2 outTexcoord;	//	テクスチャ座標


//	プログラムからの入力
//layout (location = 0) uniform float timer;	//	時間


void main() {
	outTexcoord = inTexcoord;

	gl_Position = vec4(inPosition, 1);
}