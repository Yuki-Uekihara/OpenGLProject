/*
 *	@file	standard.vert
 */
#version 450

//	入力
layout (location = 0) in vec3 inPosition;	//	頂点座標

//	出力
layout (location = 0) out vec4 outColor;	//	頂点色


//	プログラムからの入力
layout (location = 0) uniform float timer;	//	時間


void main() {

	const vec4 colors[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },		//	赤
		{ 0.0f, 1.0f, 0.0f, 1.0f },		//	緑
		{ 0.0f, 0.0f, 1.0f, 1.0f },		//	青
	};

	int i = gl_VertexID + int(timer);

	//outColor = colors[gl_VertexID % 3];
	outColor = colors[i % 3];

	gl_Position = vec4(inPosition, 1);
}