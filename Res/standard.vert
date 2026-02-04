/*
 *	@file	standard.vert
 */
#version 450

//	入力
layout (location = 0) in vec3 inPosition;	//	頂点座標
layout (location = 1) in vec2 inTexcoord;	//	テクスチャ座標
layout (location = 2) in vec3 inNormal;		//	法線

//	出力
layout (location = 0) out vec3 outPosition;	//	ワールド座標
layout (location = 1) out vec2 outTexcoord;	//	テクスチャ座標
layout (location = 2) out vec3 outNormal;	//	法線

//	プログラムからの入力
//layout (location = 0) uniform float timer;	//	時間
layout (location = 0) uniform vec3 scale;		//	拡大率
layout (location = 1) uniform vec3 position;	//	平行移動
layout (location = 2) uniform vec4 sinCosXY;		//	X,Y軸回転
layout (location = 3) uniform vec2 aspectRatioAndScaleFov;	//	アスペクト比と視野角
layout (location = 4) uniform vec3 cameraPosition;	//	カメラの平行移動
layout (location = 5) uniform vec2 cameraSinCosY;	//	カメラのY軸回転

//	親の座標変換パラメータ
layout (location = 10) uniform vec3 parentScale;	//	親の拡大率
layout (location = 11) uniform vec3 parentPosition;	//	親の平行移動
layout (location = 12) uniform vec4 parentSinCosXY;	//	親のX,Y軸回転

/*
 *	ベクトルを回転させる
 *	@param	v		ベクトル
 *	@param	sinCosX	X軸回転に使用する sin と cos の値
 *	@param	sinCosX	Y軸回転に使用する sin と cos の値
 *	@return Vector3	x軸→y軸の順番で回転させたベクトル
 */
vec3 RotateXY(vec3 v, vec2 sinCosX, vec2 sinCosY) {
	//	X軸回転
	v.zy = vec2(
		v.z * sinCosX.y + v.y * sinCosX.x,
		v.z * -sinCosX.x + v.y * sinCosX.y
	);

	//	Y軸回転
	v.xz = vec2(
		v.x * sinCosY.y + v.z * sinCosY.x,
		v.x * -sinCosY.x + v.z * sinCosY.y
	);

	return v;
}

void main() {
	outTexcoord = inTexcoord;

	//	ローカル座標系からワールド座標系に変換
	vec3 pos = inPosition * scale;

	gl_Position.xyz = RotateXY(pos, sinCosXY.xy, sinCosXY.zw);
	gl_Position.xyz += position;	//	スウィズリング(swizzling)
	
	//	親の座標変換
	gl_Position.xyz *= parentScale;
	gl_Position.xyz = RotateXY(gl_Position.xyz, parentSinCosXY.xy, parentSinCosXY.zw);
	gl_Position.xyz += parentPosition;

	outPosition = gl_Position.xyz;

	//	ワールド法線を計算
	outNormal = RotateXY(inNormal, sinCosXY.xy, sinCosXY.zw);

	//	親の座標変換
	outNormal = RotateXY(outNormal, parentSinCosXY.xy, parentSinCosXY.zw);

	//	ワールド座標系からビュー座標系に変換
	pos = gl_Position.xyz - cameraPosition;
	float cameraSinY = cameraSinCosY.x;
	float cameraCosY = cameraSinCosY.y;
	gl_Position.x = pos.x * cameraCosY + pos.z * cameraSinY;
	gl_Position.y = pos.y;
	gl_Position.z = pos.x * -cameraSinY + pos.z * cameraCosY;

	//	ビュー座標系からクリップ座標系に変換
	gl_Position.xy *= aspectRatioAndScaleFov;

	//	深度値の計算結果が -1 ~ +1 になるようにA, Bを計算
	const float near = 0.35f;
	const float far = 1000.0f;
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);

	//	遠近法を有効にする
	gl_Position.w = -gl_Position.z;
	gl_Position.z = -gl_Position.z * B + A;	//	深度値の補正
}