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
	char buf[4] = "";
	int rsize;
	printf("send1(%02d,%02d,%02d,%02d)\n", data.senddata[0], data.senddata[1], data.senddata[2], data.senddata[3]);
	fflush(stdout);
	write(data.to, data.senddata, sizeof(data.senddata));

	while(1) {
		rsize = recv(data.from, buf, sizeof(buf), 0);
		if(0 == rsize) {
			break;
		} else if(-1 == rsize) {
			perror("recv");
			break;
		} else {
			printf("send2(%02d,%02d,%02d,%02d)\n", buf[0], buf[1], buf[2], buf[3]);
			fflush(stdout);
			write(data.to, buf, rsize);
		}
	}
	close(data.from);
	close(data.to);
	return 0;
}

void Session::Start()
{
	if(setCount != 2) throw "not ready";
	th_arg[0].from = fds[0];
	th_arg[0].to = fds[1];
	th_arg[0].senddata[0] = 4;
	th_arg[0].senddata[1] = 0;
	pthread_create(&thid[0], 0, EchoThread, &th_arg[0]);

	th_arg[1].from = fds[1];
	th_arg[1].to = fds[0];
	th_arg[1].senddata[0] = 4;
	th_arg[1].senddata[1] = 1;
	pthread_create(&thid[1], 0, EchoThread, &th_arg[1]);
}

