/*
 *	@file	Random.h
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

/*
 *	乱数生成機能を格納する名前空間
 */
namespace Random {
	/*
	 *	乱数を初期化する
	 */
	void Initialize(int seed);

	/*
	 *	0.0 ~ 1.0 の範囲で一様乱数を取得する
	 */
	float Value01();

	/*
	 *	指定された値の範囲で一様乱数を取得する
	 *	@param[in]	float min
	 *	@param[in]	float max
	 */
	float Range(float min, float max);
}

#endif // !_RANDOM_H_
