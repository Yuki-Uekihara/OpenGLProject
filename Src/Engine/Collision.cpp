/*
 *  @file   Collision.cpp
 */

#include "Collision.h"
#include <algorithm>

/*
 *  AABB同士の交差判定
 *  @param AABB a   判定対象1
 *  @param AABB b   判定対象2
 *  @param Vector3 penetration 貫通ベクトル
 */ 
bool Intersect(const AABB& a, const AABB& b, Vector3& penetration) {
    //  aの左側面がbの右側面より右にある場合は交差していない
    const float dx0 = b.max.x - a.min.x;
    if (dx0 <= 0)
        return false;

    //  aの右側面がbの左側面より左にある場合は交差していない
    const float dx1 = a.max.x - b.min.x;
    if (dx1 <= 0)
        return false;

    //  aの下面がbの上面より上にある場合は交差していない
    const float dy0 = b.max.y - a.min.y;
    if (dy0 <= 0)
        return false;

    //  aの上面がbの下面より下にある場合は交差していない
    const float dy1 = a.max.y - b.min.y;
    if (dy1 <= 0)
        return false;

    //  aの奥川面がbの手前側面より手前にある場合は交差していない
    const float dz0 = b.max.z - a.min.z;
    if (dz0 <= 0)
        return false;

    //  aの手前側面がbの奥川面より奥にある場合は交差していない
    const float dz1 = a.max.z - b.min.z;
    if (dz1 <= 0)
        return false;

     
    //  交差が確定
    //  XYZ軸で交差している距離が最も短い軸を計算
    Vector3 length = { dx1, dy1, dz1 }; //  貫通距離の絶対値
    Vector3 signedLength = length;      //  符号付き

    if (dx0 < dx1) {
        length.x = dx0;
        signedLength.x = -dx0;
    }
    if (dy0 < dy1) {
        length.y = dy0;
        signedLength.y = -dy0;
    }
    if (dz0 < dz1) {
        length.z = dz0;
        signedLength.z = -dz0;
    }

    if (length.x < length.y) {
        if (length.x < length.z) {
            penetration = { signedLength.x, 0, 0 };
            return true;
        }
    }
    else if (length.y < length.z) {
        penetration = { 0, signedLength.y, 0 };
        return true;
    }
    penetration = { 0, 0, signedLength.z };
    return true;
}

/*
 *  球同士の交差判定
 *  @param Sphere a   判定対象1
 *  @param Sphere b   判定対象2
 *  @param Vector3 penetration 貫通ベクトル
 */
bool Intersect(const Sphere& a, const Sphere& b, Vector3& penetration) {
    //  2つの球の中心点の距離を求める
    const Vector3 v = b.position - a.position;  //  aの中心点からbの中心点に向かうベクトル
    const float ll = Vector3::Dot(v, v);        //  vベクトルの長さの2乗

    //  ll が2つの球の半径の合計よりも長い場合は交差していない
    const float r = a.radius + b.radius;        //  aとbの半径の合計
    if (ll > r * r)                             //  √を避けるため2乗同士で比較する
        return false;

    //  交差しているので貫通ベクトルを求める
    const float l = std::sqrtf(ll);             //  「長さの2乗」から「長さ」に変換
    const float t = (r - l) / l;                //  「半径の合計 - 長さ」の「長さに対する比率」を計算
    penetration = v * t;                        //  貫通ベクトルを求める
    return true;
}

/*
 *  AABBから点への最近点
 *  @param[in]  AABB        判定対象のAABB
 *  @param[in]  Vector3     判定対象の点
 */
Vector3 ClosestPoint(const AABB& aabb, const Vector3& point) {
    Vector3 result;
    for (int i = 0; i < 3; i++) {
        result[i] = std::clamp(point[i], aabb.min[i], aabb.max[i]);
    }
    return result;
}

/*
 *  AABBと球の交差判定
 *  @param AABB aabb   判定対象1
 *  @param Sphere sphere   判定対象2
 *  @param Vector3 penetration 貫通ベクトル
 */
bool Intersect(const AABB& aabb, const Sphere& sphere, Vector3& penetration) {
    const Vector3 p = ClosestPoint(aabb, sphere.position);
    const Vector3 v = sphere.position - p;      //   最近点から球の中心点に向かうベクトル
    const float ll = Vector3::Dot(v, v);        //  長さの2乗

    //  最近点までの距離が球体の半径よりも長ければ交差していない
    if(ll > sphere.radius * sphere.radius)
        return false;

    //  交差しているので貫通ベクトルを求める
    //  距離0より大きい場合、球体の中心はAABBの外側にある
    if (ll > 0) {
        //  球の中心座標から最近点へ向かう方向から衝突したとみなす
        const float l = std::sqrtf(ll);
        penetration = v * ((sphere.radius - l) / l);
    }
    //  距離が0の場合、球体の中心はAABBの内側にある
    else {
        //  貫通距離が最も短い面から衝突したとみなす
        int faceIndex = 0;      //  貫通方向を示すインデックス変数
        float distance = FLT_MAX;   //  貫通距離
        for (int i = 0; i < 3; i++) {
            float t0 = p[i] - aabb.min[i];
            if (t0 < distance) {
                faceIndex = i * 2;
                distance = t0;
            }

            float t1 = aabb.max[i] - p[i];
            if (t1 < distance) {
                faceIndex = i * 2 + 1;
                distance = t1;
            }
        }

        //  「AABBが球体に対してどれだけ貫通しているか」を示すベクトルが欲しいので
        //  面の外向きのベクトルを用意する
        static const Vector3 faceNormals[] = {
            Vector3::left, Vector3::right,      //  -X, +X
            Vector3::down, Vector3::up,         //  -Y, +Y
            Vector3::back, Vector3::forward     //  -Z, +Z
        };
        penetration = faceNormals[faceIndex] * distance;
    }
    return true;
}

/*
 *  スラブ(ある軸に垂直な2平面に囲まれた範囲)と光線の交差判定
 *  @param[in]  min         スラブの開始距離
 *  @param[in]  max         スラブの終了距離
 *  @param[in]  start       光線の発射点
 *  @param[in]  direction   光線の向き
 *  @param[out] tmin        AABBと光線の交差開始距離
 *  @param[out] tmax        AABBと光線の交差終了距離
 *  @return     bool
 */
bool IntersectSlab(float min, float max, float start, float direction, float& tmin, float& tmax) {
    //  光線がスラブと平行な場合 -> 発射点がスラブの中にあるかどうかでチェック
    if (abs(direction) < 0.001f) {
        return (start >= min && start <= max);
    }

    //  光線とスラブが交差する開始時刻と終了時刻を求める
    float t0 = (min - start) / direction;
    float t1 = (max - start) / direction;

    //  時刻の速いほうを開始時刻とする
    if (t0 > t1)
        std::swap(t0, t1);

    //  共通の交差範囲を求める
    //  以前の開始時刻と今回の開始時刻を比較し、遅い方を選択する
    if (t0 > tmin)
        tmin = t0;

    //  以前の開始時刻と今回の開始時刻を比較し、早い方を選択する
    if (t1 < tmax)
        tmax = t1;

    //  開始時刻 <= 終了時刻　の場合は交差している
    return tmin <= tmax;
}

/*
 *  AABBと光線の交差判定
 *  @param[in]  aabb        判定対象のAABB
 *  @param[in]  ray         判定対象の光線
 *  @param[out] distance    光線がAABBと最初に交差する距離
 *  @return     bool
 */
bool Intersect(const AABB& aabb, const Ray& ray, float& distance) {
    //  共通の交差範囲
    float tmin = 0;
    float tmax = FLT_MAX;

    //  Xスラブとの交差
    if (!IntersectSlab(aabb.min.x, aabb.max.x, ray.origin.x, ray.direction.x, tmin, tmax))
        return false;

    //  Yスラブとの交差
    if (!IntersectSlab(aabb.min.y, aabb.max.y, ray.origin.y, ray.direction.y, tmin, tmax))
        return false;

    //  Zスラブとの交差
    if (!IntersectSlab(aabb.min.z, aabb.max.z, ray.origin.z, ray.direction.z, tmin, tmax))
        return false;

    //  交点までの距離を設定
    distance = tmin;
    return true;        //  交差している
}

bool Intersect(const Sphere& sphere, const Ray& ray, float& distance) {
    //	Ray = 始点P, 向きD, 任意の位置変数t
    //	Ray(t) = P + t * D

    //	Sphere = 中心C, 半径r, 任意の座標X
    //	dot((X - C), (X - C)) = r * r

    //	球体と光線が交差する座標 = X に Ray(t) を代入
    //	dot((P + t * D - C), (P + t * D - C ) = r * r
    //	P - C を m とする
    //	dot((tD + m), (tD + m)) = rr
    //	展開
    //	dot(D, D) * t^2 + 2(dot(m, D))t + dot(m, m) = r * r
    //	D = 方向ベクトル = 長さは1
    //	t^2 + 2(dot(m, D))t + dot(m, m) = r^2
    //	解の公式
    //	t = -b ±√(b^2 - c)
    //	b = dot(m, D), c = dot(m, m) - r^2
    //	※Rayには始点があるため、始点より手前の解は除外する
    //	※t > 0


    return false;
}
