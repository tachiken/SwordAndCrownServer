#include <iostream>
#include <string>
#include <cstring>
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
}

Session::~Session()
{
	if(fds[0] != 0) close(fds[0]);
	if(fds[1] != 0) close(fds[1]);
}

void Session::Start()
{
	if(setCount != 2) throw "not ready";

	char buf[4] = {4, 0, 0, 0};
	int rsize;
	int nfds;

	printf("send1(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
	fflush(stdout);
	write(fds[0], buf, sizeof(buf));

	buf[1] = 1;
	printf("send1(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
	fflush(stdout);
	write(fds[1], buf, sizeof(buf));

	fd_set tempfds, readfds;
	FD_ZERO(&tempfds);
	FD_SET(fds[0], &tempfds);
	FD_SET(fds[1], &tempfds);
	nfds = (fds[0] > fds[1] ? fds[0] : fds[1]) + 1;

	while(1) {
		memcpy(&readfds, &tempfds, sizeof(fd_set));
		select(nfds, &readfds, NULL, NULL, 0);

		if(FD_ISSET(fds[0], &readfds)) {
			rsize = recv(fds[0], buf, sizeof(buf), 0);
			if(0 == rsize) {
				break;
			} else if(-1 == rsize) {
				perror("recv");
				break;
			} else {
				printf("send_to(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
				fflush(stdout);
				write(fds[1], buf, rsize);
			}
		}

		if(FD_ISSET(fds[1], &readfds)) {
			rsize = recv(fds[1], buf, sizeof(buf), 0);
			if(0 == rsize) {
				break;
			} else if(-1 == rsize) {
				perror("recv");
				break;
			} else {
				printf("send_from(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
				fflush(stdout);
				write(fds[0], buf, rsize);
			}
		}
	}
	close(fds[0]);
	close(fds[1]);
	return;
}

