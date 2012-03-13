#include <iostream>
#include <string>
#include <sys/socket.h>
#include "session.h"

Session::Session() : setCount(0)
{
	fds[0] = 0;
	fds[1] = 0;
}

void Session::SetFd(int fd) {
	if(setCount >= 2) throw "already session established.";
	if(fd <= 0) throw "illegal fd.";
	fds[setCount] = fd;
	setCount++;
	if(setCount >= 2) {
		Start();
	}
}

Session::~Session()
{
	if(fds[0] != 0) close(fds[0]);
	if(fds[1] != 0) close(fds[1]);
}

void* EchoThread(void* arg)
{
	fds_data data = *((fds_data*)arg);
	char buf[4] = {4, 0, 0, 0};
	int rsize;
	int nfds;

	printf("send1(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
	fflush(stdout);
	write(data.from, buf, sizeof(buf));

	buf[1] = 1;
	printf("send1(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
	fflush(stdout);
	write(data.to, buf, sizeof(buf));

	fd_set tempfds, fds;
	FD_ZERO(&tempfds);
	FD_SET(data.from, &tempfds);
	FD_SET(data.to, &tempfds);
	nfds = (data.to > data.from ? data.to : data.from) + 1;

	while(1) {
		memcpy(&fds, &tempfds, sizeof(fd_set));
		select(nfds, &fds, NULL, NULL, 0);

		if(FD_ISSET(data.from, &fds)) {
			rsize = recv(data.from, buf, sizeof(buf), 0);
			if(0 == rsize) {
				break;
			} else if(-1 == rsize) {
				perror("recv");
				break;
			} else {
				printf("send_to(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
				fflush(stdout);
				write(data.to, buf, rsize);
			}
		}

		if(FD_ISSET(data.to, &fds)) {
			rsize = recv(data.to, buf, sizeof(buf), 0);
			if(0 == rsize) {
				break;
			} else if(-1 == rsize) {
				perror("recv");
				break;
			} else {
				printf("send_from(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
				fflush(stdout);
				write(data.from, buf, rsize);
			}
		}
	}
	close(data.from);
	data.from = 0;
	close(data.to);
	data.to = 0;
	return 0;
}

void Session::Start()
{
	if(setCount != 2) throw "not ready";
	th_arg.from = fds[0];
	th_arg.to = fds[1];
	pthread_create(&thid, 0, EchoThread, &th_arg);
}

