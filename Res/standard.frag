/*
 *	@file	standard.frag
 */
#version 450

//	入力
layout (location = 0) in vec3 inPosition;	//	ワールド座標
layout (location = 1) in vec2 inTexcoord;	//	テクスチャ座標
layout (location = 2) in vec3 inNormal;		//	法線

//	出力
out vec4 outColor;	//	色

//	プログラムからの入力
layout (binding = 0) uniform sampler2D texColor;
layout (location = 100) uniform vec4 color;		//	色

//	点光源 ポイントライト
struct PointLight{
	vec3 color;		//	色と明るさ
	vec3 position;	//	位置
};
layout (location = 101) uniform PointLight pointLight;

void main() {
	vec4 c = texture(texColor, inTexcoord);

	outColor = c * color;

	//	ポイントライトの方向
	vec3 direction = pointLight.position - inPosition;

	//	方向を正規化し、長さを1にする

	//	線形補間によって長さが1ではなくなるので、再び正規化し長さを1にする

	//	ランベルトの余弦則を使って明るさを計算

	//	ランバート反射による反射光のエネルギー量を入射光と等しくする

	//	拡散光の明るさを計算

	//	拡散光の影響を反映


}