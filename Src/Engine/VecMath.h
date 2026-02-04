#ifndef _VECMATH_H_
#define _VECMATH_H_

#include <cmath>
#include <algorithm>

#define Pi		(3.1415926535f)
#define Deg2Rad (Pi / 180.0f)
#define Rad2Deg (180.0f / Pi)

//	前方宣言
struct Vector2;
struct Vector3;
struct Vector4;

struct Vector2 {
	//	メンバ変数
	float x, y;

	//	静的メンバ変数
	static Vector2 zero, one;
	static Vector2 up, down, right, left;

	//	コンストラクタ
	Vector2()
		: x(0.0f), y(0.0f) {
	}
	Vector2(float x, float y)
		: x(x), y(y) {
	}
	explicit constexpr Vector2(const Vector3& v);
	explicit constexpr Vector2(const Vector4& v);

	float operator [] (size_t i) const;
	float& operator [] (size_t i);
};

struct Vector3 {
	//	メンバ変数
	float x, y, z;

	//	静的メンバ変数
	static Vector3 zero, one;
	static Vector3 up, down, right, left, forward, back;

	//	コンストラクタ
	constexpr Vector3()
		:x(0.0f), y(0.0f), z(0.0f) {
	}
	constexpr Vector3(float x, float y, float z)
		: x(x), y(y), z(z) {
	}
	explicit constexpr Vector3(const Vector4& v);

	//	演算子
	Vector3 operator - ();

	Vector3 operator * (float v) const;
	Vector3 operator / (float v) const;
	Vector3 operator + (const Vector3& v) const;
	Vector3 operator - (const Vector3& v) const;

	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *= (float v);
	Vector3& operator /= (float v);

	bool operator == (const Vector3& v) const;
	bool operator != (const Vector3& v) const;

	float operator [] (size_t i) const;
	float& operator [] (size_t i);

	//	演算
	static Vector3 Scale(const Vector3& v1, const Vector3& v2);
	static float Dot(const Vector3& v1, const Vector3& v2);
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	float Magnitude() const;
	Vector3 Normalized() const;
};

struct Vector4 {
	//	メンバ変数
	float x, y, z, w;

	//	静的メンバ変数
	static Vector4 zero, one;

	//	コンストラクタ
	constexpr Vector4()
		:x(0.0f), y(0.0f), z(0.0f), w(0.0f) {
	}
	constexpr Vector4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {
	}
	constexpr Vector4(const Vector3& v, float w = 1.0f)
		: x(v.x), y(v.y), z(v.z), w(w) {
	}

	//	演算子
	Vector4 operator * (float v) const;
	Vector4 operator / (float v) const;
	Vector4 operator + (const Vector4& v) const;
	Vector4 operator + (const Vector3& v) const;
	Vector4 operator - (const Vector4& v) const;
	Vector4 operator - (const Vector3& v) const;

	Vector4& operator += (const Vector4& v);
	Vector4& operator += (const Vector3& v);
	Vector4& operator -= (const Vector4& v);
	Vector4& operator -= (const Vector3& v);

	float operator [] (size_t i) const;
	float& operator [] (size_t i);

	//	演算
	static Vector4 Scale(const Vector4& v1, const Vector4& v2);


	//	座標変換ベクトルの配列を計算する
	void GetTransformVectors(Vector4* result,
		const Vector3& scale, const Vector3& rotation, const Vector3& position);

	//	座標変換ベクトルの配列をかけ合わせる
	void MultiplyTransformVectors(Vector4* result,
		const Vector4* m, const Vector4* n);

	//	法線変換ベクトルの配列を計算する
	void GetRotationVectors(Vector3* result,
		const Vector3& rotation);

	//	法線変換ベクトルの配列をかけ合わせる
	void MultiplayRotationVectors(Vector3* result,
		const Vector3* m, const Vector3* n);
};



#endif // !_VECMATH_H_
