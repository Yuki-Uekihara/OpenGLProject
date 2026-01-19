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

//	ライト
struct Light {
	vec4 colorAndFalloffAgnle[16];	//	色と明るさ
	vec4 positionAndRadius[16];		//	位置と半径
	vec4 directionAndConeAngle[16];	//	向き
};

layout (location = 110) uniform int lightCount;		//	ライトの数
layout (location = 111) uniform Light light;

void main() {
	vec4 c = texture(texColor, inTexcoord);

	//	テクスチャのガンマ補正
	const float crtGamma = 2.2;		//	モニターのガンマ値
	c.rgb = pow(c.rgb, vec3(crtGamma));

	outColor = c * color;

	vec3 diffuse = vec3(0);	//	拡散光の明るさの合計
	for (int i = 0; i < lightCount; i++) {
		//	ポイントライトの方向
		vec3 direction = light.positionAndRadius[i].xyz - inPosition;
	
		//	光源までの距離
		float sqrDistance = dot(direction, direction);
		float distance = sqrt(sqrDistance);
	
		//	方向を正規化し、長さを1にする
		direction = normalize(direction);
	
		//	線形補間によって長さが1ではなくなるので、再び正規化し長さを1にする
		vec3 normal = normalize(inNormal);
	
		//	ランベルトの余弦則を使って明るさを計算
		float theta = max(dot(direction, normal), 0);
	
		//	ランバート反射による反射光のエネルギー量を入射光と等しくする
		float illuminance = theta / 3.14159265f;
	
		//	照射角度が0より大きければスポットライトとみなす
		const float coneAngle = light.directionAndConeAngle[i].w;
		if(coneAngle > 0){
			//	「ライトからフラグメントへ向かうベクトル」と「スポットライトのベクトル」のなす角を計算
			float angle = acos(dot(-direction, light.directionAndConeAngle[i].xyz));
			//	角度が coneAngle 以上なら範囲外
			if(angle >= coneAngle)
				continue;

			//	最大照射角度のときに0、減衰開始角度のときに1になるように補間する
			const float falloffAngle = light.colorAndFalloffAgnle[i].w;
			const float a = min((coneAngle - angle) / (coneAngle - falloffAngle), 1);
			illuminance *= 1;
		}

		//	ライトの最大距離を制限
		const float radius = light.positionAndRadius[i].w;
		const float smoothFactor = clamp(1 - pow(distance / radius, 4), 0, 1);
		illuminance *= smoothFactor * smoothFactor;
	
		//	逆2乗の法則によって明るさを減衰させる
		illuminance /= sqrDistance + 1;
	
		//	拡散光の明るさを計算
		diffuse += light.colorAndFalloffAgnle[i].xyz * illuminance;
	}

	//	拡散光の影響を反映
	outColor.rgb *= diffuse;

	//	ガンマ補正を戻す
	outColor.rgb = pow(outColor.rgb, vec3(1 / crtGamma));
}