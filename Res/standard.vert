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
layout (location = 0) uniform vec3 scale;		//	拡大率
layout (location = 1) uniform vec3 position;	//	平行移動
layout (location = 2) uniform vec2 sinCosY;		//	Y軸回転

void main() {
	outTexcoord = inTexcoord;

	//	拡縮
	vec3 pos = inPosition * scale;

	//	回転(Y軸)
	float sinY = sinCosY.x;
	float cosY = sinCosY.y;
	gl_Position.x = pos.x * cosY + pos.z * sinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -sinY + pos.z * cosY;

	//	平行移動
	gl_Position.xyz += position;	//	スウィズリング(swizzling)
	
	//	遠近法を有効にする
	gl_Position.zw = -gl_Position.zz;
}