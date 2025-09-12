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
layout (location = 3) uniform float aspectRatio;	//	アスペクト比
layout (location = 4) uniform vec3 cameraPosition;	//	カメラの平行移動
layout (location = 5) uniform vec2 cameraSinCosY;	//	カメラのY軸回転


void main() {
	outTexcoord = inTexcoord;

	//	ローカル座標系からワールド座標系に変換
	vec3 pos = inPosition * scale;

	float sinY = sinCosY.x;
	float cosY = sinCosY.y;
	gl_Position.x = pos.x * cosY + pos.z * sinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -sinY + pos.z * cosY;

	gl_Position.xyz += position;	//	スウィズリング(swizzling)
	
	//	ワールド座標系からビュー座標系に変換
	pos = gl_Position.xyz - cameraPosition;
	float cameraSinY = cameraSinCosY.x;
	float cameraCosY = cameraSinCosY.y;
	gl_Position.x = pos.x * cameraCosY + pos.z * cameraSinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -cameraSinY + pos.z * cameraCosY;

	//	ビュー座標系からクリップ座標系に変換
	gl_Position.x /= aspectRatio;

	//	深度値の計算結果が -1 ~ +1 になるようにA, Bを計算
	const float near = 0.5f;
	const float far = 1000.0f;
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);

	//	遠近法を有効にする
	gl_Position.w = -gl_Position.z;
	gl_Position.z = -gl_Position.z * B + A;	//	深度値の補正
}