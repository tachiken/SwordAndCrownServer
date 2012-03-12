#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "session.h"
using namespace std;

#define PORT 9173
#define MAXDATA 1024

int main()
{
	struct sockaddr_in saddr;
	struct sockaddr_in caddr;

	int listen_fd;
	int conn_fd;

	socklen_t len = sizeof(struct sockaddr_in);

	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	memset((char*)&saddr, 0x00, sizeof(saddr));

	saddr.sin_family = PF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(PORT);
	if(bind(listen_fd, (struct sockaddr*)&saddr, len) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if(listen(listen_fd, SOMAXCONN) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Start Listening Port : %d...\n", PORT);
	fflush(stdout);

	while(1) {
		Session* s = new Session();
		if((conn_fd = accept(listen_fd, (struct sockaddr*)&caddr, &len)) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("First accept completed.\n");
		fflush(stdout);
		s->SetFd(conn_fd);

		if((conn_fd = accept(listen_fd, (struct sockaddr*)&caddr, &len)) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("Second accept completed.\n");
		fflush(stdout);
		s->SetFd(conn_fd);
	}
	close(listen_fd);

	printf("Server ended.\n");
	return 0;
}

