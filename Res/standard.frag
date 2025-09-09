/*
 *	@file	standard.frag
 */
#version 450

//	入力
layout (location = 1) in vec2 inTexcoord;	//	テクスチャ座標

//	出力
out vec4 outColor;	//	色

//	プログラムからの入力
layout (binding = 0) uniform sampler2D texColor;
layout (location = 100) uniform vec4 color;		//	色




void main() {
	vec4 c = texture(texColor, inTexcoord);

	outColor = c * color;
}