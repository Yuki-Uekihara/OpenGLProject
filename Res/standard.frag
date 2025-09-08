/*
 *	@file	standard.frag
 */
#version 450

//	入力
layout (location = 0) in vec4 inColor;	//	頂点色

//	出力
out vec4 outColor;	//	色

//	プログラムからの入力
layout (binding = 0) uniform sampler2D texColor;


void main() {
	vec4 c = texture(texColor, gl_FragCoord.xy * 0.01f);

	outColor = c * inColor;
	//outColor = inColor;
	//outColor = vec4(0.2f, 0.3f, 1.0f, 1.0f);
}