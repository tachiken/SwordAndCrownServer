#ifndef SESSION_H_
#define SESSION_H_
#include <pthread.h>

#define MSGLEN 16

class Session {
private:
	int fds[2];
	int setCount;
	int id;
	void DumpMessage(const char* caption, char* data);
	void DumpUsername(int user, char* data);
public:
	Session(int id);
	~Session();
	void SetFd(int fd);
	void Start();
	int GetId() { return id; };
};

#endif /* SESSION_H_ */
