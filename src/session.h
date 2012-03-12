#ifndef SESSION_H_
#define SESSION_H_
#include <pthread.h>

typedef struct {
	int from;
	int to;
} fds_data;


class Session {
private:
	int setCount;
	pthread_t thid;
	fds_data th_arg;

	void Start();
public:
	int fds[2];

	Session();
	~Session();
	void SetFd(int fd);
};

#endif /* SESSION_H_ */
