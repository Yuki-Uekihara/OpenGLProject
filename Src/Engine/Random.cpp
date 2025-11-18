/*
 *	@file Random.cpp
 */

#include "Random.h"
#include <random>

namespace Random {
	//	乱数生成オブジェクト
	std::random_device rd;
	std::mt19937 randomEngine(rd());

	/*
	 *	乱数を初期化する
	 */
	void Initialize(int seed) {
		randomEngine.seed(seed);
	}

	/*
	 *	0.0 ~ 1.0 の範囲で一様乱数を取得する
	 */
	float Value01() {
		return std::uniform_real_distribution<float>(0.0f, 1.0f)(randomEngine);
	}

	/*
	 *	指定された値の範囲で一様乱数を取得する
	 *	@param[in]	float min
	 *	@param[in]	float max
	 */
	float Range(float min, float max) {
		return std::uniform_real_distribution<float>(min, max)(randomEngine);
	}

}