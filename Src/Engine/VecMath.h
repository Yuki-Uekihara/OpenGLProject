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

struct Matrix3x3;
struct Matrix4x4;


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


};


struct Matrix3x3 {
	Vector3 data[3];

	static Matrix3x3 identity;

	Matrix3x3() = default;
	Matrix3x3(const Vector3& v0, const Vector3& v1, const Vector3& v2) 		{
		data[0] = v0;
		data[1] = v1;
		data[2] = v2;
	}
	explicit Matrix3x3(const Matrix4x4& m);


	//	添え字演算子
	Vector3& operator [] (size_t i);
	const Vector3& operator [] (size_t i) const;


	Matrix3x3 Transposed() const {
		Matrix3x3 r;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				r.data[i][j] = data[j][i];
			}
		}
		return r;
	}
};

struct Matrix4x4 {
	Vector4 data[4];

	static Matrix4x4 identity;

	Matrix4x4() = default;
	Matrix4x4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3) {
		data[0] = v0;
		data[1] = v1;
		data[2] = v2;
		data[3] = v3;
	}
	explicit Matrix4x4(const Matrix3x3& m) {
		data[0] = { m.data[0], 0 };
		data[1] = { m.data[1], 0 };
		data[2] = { m.data[2], 0 };
		data[3] = { 0, 0, 0, 1 };
	}

	//	添え字演算子
	Vector4& operator [] (size_t i);
	const Vector4& operator [] (size_t i) const;
};

//	Matrix3x3 * Vector3
inline Vector3 operator * (const Matrix3x3& m, const Vector3& v) {
	return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z;
}

//	Matrix3x3 * Matrix3x3
inline Matrix3x3 operator * (const Matrix3x3& m, const Matrix3x3& n) {
	Matrix3x3 mn;
	mn.data[0] = m * n.data[0];
	mn.data[1] = m * n.data[1];
	mn.data[2] = m * n.data[2];
	return mn;
}

//	Matrix3x3 *= Matrix3x3
inline Matrix3x3& operator *= (Matrix3x3& m, const Matrix3x3& n) {
	m = m * n;
	return m;
}

//	Matrix4x4 * Vector4
inline Vector4 operator * (const Matrix4x4& m, const Vector4& v) {
	return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z + m.data[3] * v.w;
}

//	Matrix4x4 * Matrix4x4
inline Matrix4x4 operator * (const Matrix4x4& m, const Matrix4x4& n) {
	Matrix4x4 mn;
	mn.data[0] = m * n.data[0];
	mn.data[1] = m * n.data[1];
	mn.data[2] = m * n.data[2];
	mn.data[3] = m * n.data[3];
	return mn;
}

//	Matrix4x4 *= Matrix4x4
inline Matrix4x4& operator *= (Matrix4x4& m, const Matrix4x4& n) {
	m = m * n;
	return m;
}

//	回転角度から回転行列を取得
Matrix3x3 GetRotationMatrix(const Vector3& rotation);

//	拡大率、回転角度、平行移動から座標変換行列を取得
Matrix4x4 GetTransformMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& position);


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
void MultiplyRotationVectors(Vector3* result,
	const Vector3* m, const Vector3* n);


#endif // !_VECMATH_H_
