#include "VecMath.h"
#include <cmath>
#include <algorithm>

#pragma region Operator

//	静的メンバ変数の宣言
Vector2 Vector2::zero = { 0.0f,  0.0f };
Vector2 Vector2::one = { 1.0f,  1.0f };
Vector2 Vector2::up = { 0.0f, 1.0f };
Vector2 Vector2::down = { 0.0f, -1.0f };
Vector2 Vector2::right = { 1.0f, 0.0f };
Vector2 Vector2::left = { -1.0f, 0.0f };


//	静的メンバ変数の宣言
Vector3 Vector3::zero = { 0.0f, 0.0f, 0.0f };
Vector3 Vector3::one = { 1.0f, 1.0f, 1.0f };
Vector3 Vector3::up = { 0.0f, 1.0f, 0.0f };
Vector3 Vector3::down = { 0.0f, -1.0f, 0.0f };
Vector3 Vector3::right = { 1.0f, 0.0f, 0.0f };
Vector3 Vector3::left = { -1.0f, 0.0f, 0.0f };
Vector3 Vector3::forward = { 0.0f, 0.0f, 1.0f };
Vector3 Vector3::back = { 0.0f, 0.0f, -1.0f };

//	静的メンバ変数の宣言
Vector4 Vector4::zero = { 0.0f, 0.0f, 0.0f, 0.0f };
Vector4 Vector4::one = { 1.0f, 1.0f, 1.0f, 1.0f };

constexpr Vector2::Vector2(const Vector3& v)
	: x(v.x), y(v.y) {
}

constexpr Vector2::Vector2(const Vector4& v)
	: x(v.x), y(v.y) {
}

float Vector2::operator [] (size_t i) const {
	return *(&x + i);
}

float& Vector2::operator [] (size_t i) {
	return *(&x + i);
}

constexpr Vector3::Vector3(const Vector4& v)
	: x(v.x), y(v.y), z(v.z) {

}


Vector3 Vector3::operator - () {
	return (*this * -1);
}

Vector3 Vector3::operator * (float v) const {
	return Vector3(x * v, y * v, z * v);
}

Vector3 Vector3::operator / (float v) const {
	const float EP = FLT_EPSILON;
	if (v < EP)
		return Vector3::zero;

	return Vector3(x / v, y / v, z / v);
}


Vector3 Vector3::operator + (const Vector3& v) const {
	return Vector3(x + v.x, y + v.y, z + v.z);
}


Vector3 Vector3::operator - (const Vector3& v) const {
	return Vector3(x - v.x, y - v.y, z - v.z);
}


Vector3& Vector3::operator += (const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}


Vector3& Vector3::operator -= (const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator *= (float v) {
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

Vector3& Vector3::operator /= (float v) {
	const float EP = FLT_EPSILON;
	if (v < EP) {
		x = y = z = 0.0f;
		return *this;
	}

	x *= v;
	y *= v;
	z *= v;
	return *this;
}

bool Vector3::operator == (const Vector3& v) const {
	const float EP = FLT_EPSILON;
	return std::fabsf(x - v.x) < EP &&
		std::fabsf(y - v.y) < EP &&
		std::fabsf(z - v.z) < EP;
}

bool Vector3::operator != (const Vector3& v) const {
	return !(*this == v);
}

float Vector3::operator [] (size_t i) const {
	return *(&x + i);
}

float& Vector3::operator [] (size_t i) {
	return *(&x + i);
}

Vector3 Vector3::Scale(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
	const Vector3 vn1 = v1.Normalized();
	const Vector3 vn2 = v2.Normalized();

	return vn1.x * vn2.x + vn1.y * vn2.y + vn1.z * vn2.z;
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	const Vector3 vn1 = v1.Normalized();
	const Vector3 vn2 = v2.Normalized();

	return Vector3(
		vn1.y * vn2.z - vn1.z * vn2.y,
		vn1.z * vn2.x - vn1.x * vn2.z,
		vn1.x * vn2.y - vn1.y * vn2.x
	);
}

float Vector3::Magnitude() const {
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalized() const {
	const float l = Magnitude();
	return (*this / l);
}

Vector4 Vector4::operator * (float v) const {
	return Vector4(x * v, y * v, z * v, w * v);
}

Vector4 Vector4::operator / (float v) const {
	const float EP = FLT_EPSILON;
	if (v < EP)
		return Vector4::zero;

	return Vector4(x / v, y / v, z / v, w / v);
}

Vector4 Vector4::operator + (const Vector4& v) const {
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator + (const Vector3& v) const {
	Vector4 vv(v);
	return (*this + vv);
}

Vector4 Vector4::operator - (const Vector4& v) const {
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator - (const Vector3& v) const {
	Vector4 vv(v);
	return (*this - vv);
}

Vector4& Vector4::operator += (const Vector4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vector4& Vector4::operator += (const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector4& Vector4::operator -= (const Vector4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4& Vector4::operator -= (const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

float Vector4::operator [] (size_t i) const {
	return *(&x + i);
}

float& Vector4::operator [] (size_t i) {
	return *(&x + i);
}

Vector4 Vector4::Scale(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

/*
 *	 座標変換ベクトルの配列を計算する
 *	@param	out Vector4 演算結果
 *	@param		Vector3 拡縮
 *	@param		Vector3 回転
 *	@param		Vector3 平行移動
 */
void Vector4::GetTransformVectors(Vector4* result, const Vector3& scale, const Vector3& rotation, const Vector3& position) {
	const float sinX = std::sinf(rotation.x);
	const float cosX = std::cosf(rotation.x);
	const float sinY = std::sinf(rotation.y);
	const float cosY = std::cosf(rotation.y);

	result[0] = { scale.x * cosY , scale.y * sinX * sinY, scale.z * cosX * sinY , position.x };
	result[1] = { 0				 , scale.y * cosX		, scale.z * -sinX		, position.y };
	result[2] = { scale.x * -sinY, scale.y * sinX * cosY, scale.z * cosX * cosY , position.z };
	result[3] = { 0				 , 0					, 0						, 1			 };
}

/*
 *	 座標変換ベクトルの配列をかけ合わせる
 *	@param	out Vector4 演算結果
 *	@param		Vector4 ベクトル1
 *	@param		Vector4 ベクトル2
 */
void Vector4::MultiplyTransformVectors(Vector4* result, const Vector4* m, const Vector4* n) {
}

/*
 *	 法線変換ベクトルの配列を計算する
 *	@param	out Vector3 演算結果
 *	@param		Vector3 回転
 */
void Vector4::GetRotationVectors(Vector3* result, const Vector3& rotation) {
	const float sinX = std::sinf(rotation.x);
	const float cosX = std::cosf(rotation.x);
	const float sinY = std::sinf(rotation.y);
	const float cosY = std::cosf(rotation.y);

	result[0] = { cosY  , sinX * sinY, cosX * sinY };
	result[1] = { 0		, cosX		 , -sinX	   };
	result[2] = { -sinY , sinX * cosY, cosX * cosY };
}

/*
 *	 法線変換ベクトルの配列をかけ合わせる
 *	@param	out Vector3 演算結果
 *	@param		Vector3 ベクトル1
 *	@param		Vector3 ベクトル2
 */
void Vector4::MultiplayRotationVectors(Vector3* result, const Vector3* m, const Vector3* n) {

}

#pragma endregion
