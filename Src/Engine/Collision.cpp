/*
 *  @file   Collision.cpp
 */

#include "Collision.h"

/*
 *  AABB同士の交差判定
 *  @param AABB a   判定対象1
 *  @param AABB b   判定対象2
 *  @param Vector3 penetration 貫通ベクトル
 */ 
bool Intersect(const AABB& a, const AABB b, Vector3& penetration) {
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
