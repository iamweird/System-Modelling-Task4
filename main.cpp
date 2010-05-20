#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <list>

using namespace std;

#define TESTS		1000

#define EXP_MIN		0
#define EXP_MAX		1

#define L			(4.)

inline double random(int _min, int _max) {
	int saved = 0;
	if (_min<0) {
		_max += _min;
		saved = _min;
		_min = 0;
	}
	return ((double) rand() / (double) RAND_MAX) * _max + _min + saved;
}

inline double random_exponential(double _lambda) {
	return -log(random(EXP_MIN, EXP_MAX)) / _lambda;
}

double ztime = .0;

class demand {
public:
	double in, start, out;
public:
	demand() {
		in = start = out = .0;
	}
	demand(double lambda) {
		::ztime += random_exponential(lambda);
		in = ::ztime;
		start = out = .0;
	}
	demand(const demand &d) {
		this->in = d.in;
		this->start = d.start;
		this->out = d.out;
	}
};

void process_demands(list<demand*> *q) {
	ztime = .0;
	list<demand*>::iterator prev=NULL, i=q->begin();
	ztime += random_exponential(L);
	(*i)->start = ztime;
	prev = i;
	i++;
	for (; i!=q->end(); i++) {
		ztime += random_exponential(L);
		(*i)->start = ztime;
		(*i)->out = (*prev)->
		prev = i;
	}
}

int main(int argc, char *argv[]) {
	srand((unsigned) time(NULL));

	double l1, l2, tau;
	printf("Enter lambda1 parameter of exponential distribution: ");
	scanf("%lf", &l1);
	printf("Enter lambda2 parameter of exponential distribution: ");
	scanf("%lf", &l2);
	printf("Enter tau parameter: ");
	scanf("%lf", &tau);

	list<demand*> *q = new list<demand*>;
	for (int i=0; i<TESTS; ++i) {
		double t = .0;
		double next;
		do {
			q->push_back(new demand(l1));
			++i;
		} while (t<=tau && i<TESTS);
		t = .0;
		do {
			q->push_back(new demand(l1));
			++i;
		} while (t<=tau && i<TESTS);
	}

	return EXIT_SUCCESS;
}
