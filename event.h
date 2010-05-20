#ifndef MOD4_EVENT_H_
#define MOD4_EVENT_H_

enum modes {in, out};

struct Event {
	int mode;
	double time;
	request * req;

	bool operator < (const Event & rhs) const {
		return time < rhs.time;
	}
};

#endif
