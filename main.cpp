/*
 * main.cpp
 *
 *  Created on: 20.05.2010
 *      Author: sparxxx
 */

#include <iostream>
#include <queue>
#include <set>
#include <limits>
#include <conio.h>
#include "random.h"
#include "request.h"
#include "event.h"

#define	TESTS	1000
#define L		1.0

using namespace std;

typedef queue<request>::container_type::iterator qrit;

void request_main();
void request_in();
void request_start();
void request_out();
double next_req_time();
double next_srv_time();
bool check_conditions();
void read_input();

double L1, L2, tau;
set<Event, less<Event>> q_evt;
queue<request> q_in;
queue<request> q_out;
double T = 0.;
bool device_ready = true;
double t1 = 0.;
bool gen_thread_1 = true;
int requests_processed = 0;

double next_req_time() {
	if (t1+tau < T) {
		t1 = T;
		gen_thread_1 = !gen_thread_1;
	}
	if (gen_thread_1) {
		return random_exponential(L1);
	}
	else {
		return random_exponential(L2);
	}
}

double next_srv_time() {
	requests_processed++;
	return random_exponential(L);
}

bool check_conditions() {
	return requests_processed > TESTS;
}

void request_in() {
	request req;
	req.in = T;
	q_in.push(req);

	Event evt;
	evt.mode = in;
	evt.time = T + next_req_time();
	evt.req = NULL;
	q_evt.insert(evt);
}

void request_start() {
	device_ready = false;
	q_in.front().start = T;

	Event evt;
	evt.mode = out;
	evt.time = T + next_srv_time();
	evt.req = &(q_in.front());
	q_evt.insert(evt);
}

void request_out() {
	device_ready = true;
	request req = q_in.front();
	req.end = T;
	q_out.push(req);
	q_in.pop();
	if (!q_in.empty()) {
		request_start();
	}
}

void request_main() {
	while (1) {
		if (!q_in.empty() && device_ready) {
			request_start();
		}
		else {
			T = q_evt.begin()->time;
			if (check_conditions()) return;
			else {
				int mode = q_evt.begin()->mode;
				q_evt.erase(q_evt.begin());
				switch (mode) {
					case in:
						request_in();
						break;
					case out:
						request_out();
						break;
				}
			}
		}
	}
}

double get_expected_residence_time(queue<request> &q) {
	double time = 0.;
	int i = 0;
	for (qrit it=q.c.begin(); it != q.c.end() && i < TESTS; it++, i++) {
		time += it->end - it->in;
	}
	time /= q.size();
	return time;
}

double get_expected_requests_number(queue<request> &q) {
	int *req_num = new int[TESTS+1];
	memset(req_num, 0, sizeof(int)*(TESTS+1));

	double step = (q.back().in - q.front().in) / TESTS;
#ifdef _DEBUG
	printf("Step: %.6lf\n", step);
#endif
	for (double t=q.front().in; t < q.back().in; t+=step) {
		qrit it;
		for (it=q.c.begin(); it != q.c.end() && it->end <= t; it++);
		int reqs = 1;
		for (; it != q.c.end() && it->in <= t && it->end > t; it++, reqs++);
		req_num[reqs]++;
	}

#ifdef _DEBUG
	for (int i=0; i<TESTS+1; i++) {
		if (req_num[i]) {
			printf("%6.d: %6.d\n", i, req_num[i]);
		}
	}
#endif

	double expected_req_num = 0.;
	for (int i=0; i<TESTS+1; i++) {
		if (req_num[i]) {
			expected_req_num += i * (((double) req_num[i]) / TESTS);
		}
	}

	delete []req_num;
	return expected_req_num;
}

void read_input() {
	printf("Enter lambda1 and lambda2 parameters: ");
	scanf("%lf %lf", &L1, &L2);
	printf("Enter tau parameter: ");
	scanf("%lf", &tau);
}

int main(int argc, char * argv[]) {
	random_init();

	read_input();

	Event evt;
	evt.mode = in;
	evt.time = T + next_req_time();
	evt.req = NULL;
	q_evt.insert(evt);

	request_main();

	printf("Expected residence time is %.3lf\n", get_expected_residence_time(q_out));

	printf("Expected requests number is %.3lf\n", get_expected_requests_number(q_out));

#ifdef _DEBUG
	while (!q_out.empty()) {
		printf("%5.6lf %5.6lf %5.6lf\n",
				q_out.front().in,
				q_out.front().start,
				q_out.front().end);
		q_out.pop();
	}
#endif

#ifndef _DEBUG
	_getch();
#endif
	return 0;
}
