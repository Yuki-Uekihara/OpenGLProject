#ifndef _VECMATH_H_
#define _VECMATH_H_

#include <cmath>
#include <algorithm>

#define Pi		(3.1415926535f)
#define Deg2Rad (Pi / 180.0f)
#define Rad2Deg (180.0f / Pi)

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
	Vector4 operator - ();
	Vector4 operator * (float v) const;
	Vector4 operator / (float v) const;
	Vector4 operator + (const Vector4& v) const;
	Vector4 operator + (const Vector3& v) const;
	Vector4 operator - (const Vector4& v) const;
	Vector4 operator - (const Vector3& v) const;

	Vector4& operator +=(const Vector4& v);
	Vector4& operator +=(const Vector3& v);
	Vector4& operator -=(const Vector4& v);
	Vector4& operator -=(const Vector3& v);

	//	演算
	static Vector4 Scale(const Vector4& v1, const Vector4& v2);
};

#endif // !_VECMATH_H_
