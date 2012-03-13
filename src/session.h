#ifndef SESSION_H_
#define SESSION_H_
#include <pthread.h>

class Session {
private:
	int fds[2];
	int setCount;
public:
	Session();
	~Session();
	void SetFd(int fd);
	void Start();
};

#endif /* SESSION_H_ */
