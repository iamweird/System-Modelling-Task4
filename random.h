#ifndef MOD4_RANDOM_H_
#define MOD4_RANDOM_H_

#include <cstdlib>
#include <ctime>
#include <cmath>

// Задаем константы для экспоненциального распределения
#define EXP_MIN		0
#define EXP_MAX		1

/*
 * Функция, инициализирующая генератор случайных чисел
 * текущим значением времени.
 */
inline void random_init() {
	srand( (unsigned) time(NULL) );
}

/*
 * Функция, генерирующая случайное число
 * на отрезке [_min, _max].
 */
inline double random(int _min, int _max) {
	int saved = 0;
	// Если одна из границ отрезка отрицательна,
	// используем коррекцию.
	if (_min<0) {
		_max -= _min;
		saved = _min;
		_min = 0;
	}
	return ((double) rand() / (double) RAND_MAX) * _max + _min + saved;
}

/*
 * Функция, генерирующая случайные числа,
 * распределенные по экспоненциальному закону с параметром _lambda.
 */
inline double random_exponential(double _lambda) {
	return -log(random(EXP_MIN, EXP_MAX)) / _lambda;
}

#endif /* MOD4_RANDOM_H_ */
