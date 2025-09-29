/*
 *  @file   Collision.cpp
 */

#include "Collision.h"

/*
 *  AABB���m�̌�������
 *  @param AABB a   ����Ώ�1
 *  @param AABB b   ����Ώ�2
 *  @param Vector3 penetration �ђʃx�N�g��
 */ 
bool Intersect(const AABB& a, const AABB b, Vector3& penetration) {
    //  a�̍����ʂ�b�̉E���ʂ��E�ɂ���ꍇ�͌������Ă��Ȃ�
    const float dx0 = b.max.x - a.min.x;
    if (dx0 <= 0)
        return false;

    //  a�̉E���ʂ�b�̍����ʂ�荶�ɂ���ꍇ�͌������Ă��Ȃ�
    const float dx1 = a.max.x - b.min.x;
    if (dx1 <= 0)
        return false;

    //  a�̉��ʂ�b�̏�ʂ���ɂ���ꍇ�͌������Ă��Ȃ�
    const float dy0 = b.max.y - a.min.y;
    if (dy0 <= 0)
        return false;

    //  a�̏�ʂ�b�̉��ʂ�艺�ɂ���ꍇ�͌������Ă��Ȃ�
    const float dy1 = a.max.y - b.min.y;
    if (dy1 <= 0)
        return false;

    //  a�̉���ʂ�b�̎�O���ʂ���O�ɂ���ꍇ�͌������Ă��Ȃ�
    const float dz0 = b.max.z - a.min.z;
    if (dz0 <= 0)
        return false;

    //  a�̎�O���ʂ�b�̉���ʂ�艜�ɂ���ꍇ�͌������Ă��Ȃ�
    const float dz1 = a.max.z - b.min.z;
    if (dz1 <= 0)
        return false;

     
    //  �������m��
    //  XYZ���Ō������Ă��鋗�����ł��Z�������v�Z
    Vector3 length = { dx1, dy1, dz1 }; //  �ђʋ����̐�Βl
    Vector3 signedLength = length;      //  �����t��

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
