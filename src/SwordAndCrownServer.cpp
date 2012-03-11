#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

#define PORT 9173
#define MAXDATA 1024

int main()
{
	struct sockaddr_in saddr;
	struct sockaddr_in caddr;

	int listen_fd;
	int conn_fd;

	int len = sizeof(struct sockaddr_in);
	int rsize;
	char buf[MAXDATA];

	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	bzero((char*)&saddr, sizeof(saddr));

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

	if((conn_fd = accept(listen_fd, (struct sockaddr*)&caddr, &len)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	close(listen_fd);

	do {
		rsize = recv(conn_fd, buf, MAXDATA, 0);

		if(rsize == 0) {
			break;
		} else if (rsize == -1) {
			perror("recv");
			exit(EXIT_FAILURE);
		} else {
			write(conn_fd, buf, rsize);
		}
	} while(1);

	if(close(conn_fd) < 0) {
		perror("close");
		exit(EXIT_FAILURE);
	}

	printf("Connection closed.\n");
	return 0;
}

