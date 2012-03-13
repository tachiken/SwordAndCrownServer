#ifndef SESSION_H_
#define SESSION_H_
#include <pthread.h>

class Session {
private:
	int fds[2];
	int setCount;
	int id;
public:
	Session(int id);
	~Session();
	void SetFd(int fd);
	void Start();
	int GetId() { return id; };
};

#endif /* SESSION_H_ */
