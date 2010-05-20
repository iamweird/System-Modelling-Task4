#ifndef MOD4_RANDOM_H_
#define MOD4_RANDOM_H_

#include <cstdlib>
#include <ctime>
#include <cmath>

// ������ ��������� ��� ����������������� �������������
#define EXP_MIN		0
#define EXP_MAX		1

/*
 * �������, ���������������� ��������� ��������� �����
 * ������� ��������� �������.
 */
inline void random_init() {
	srand( (unsigned) time(NULL) );
}

/*
 * �������, ������������ ��������� �����
 * �� ������� [_min, _max].
 */
inline double random(int _min, int _max) {
	int saved = 0;
	// ���� ���� �� ������ ������� ������������,
	// ���������� ���������.
	if (_min<0) {
		_max -= _min;
		saved = _min;
		_min = 0;
	}
	return ((double) rand() / (double) RAND_MAX) * _max + _min + saved;
}

/*
 * �������, ������������ ��������� �����,
 * �������������� �� ����������������� ������ � ���������� _lambda.
 */
inline double random_exponential(double _lambda) {
	return -log(random(EXP_MIN, EXP_MAX)) / _lambda;
}

#endif /* MOD4_RANDOM_H_ */
