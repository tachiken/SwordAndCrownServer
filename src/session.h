#ifndef SESSION_H_
#define SESSION_H_
#include <pthread.h>

typedef struct {
	int from;
	int to;
	char senddata[4];
} fds_data;


class Session {
private:
	int setCount;
	pthread_t thid[2];
	fds_data th_arg[2];

	void Start();
public:
	int fds[2];

	Session();
	~Session();
	void SetFd(int fd);
};

#endif /* SESSION_H_ */
