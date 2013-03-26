#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "serv_thread_func.h"

int main(){
	int listener, sock;
	struct sockaddr_in addr;
	pthread_t pid;
	int i;
	char str[100];

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
		perror("listener socket fail");
		exit(1);
	}


	addr.sin_family = AF_INET;
	addr.sin_port = htons(3561);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_ntop(AF_INET, &(addr.sin_addr), str, 100);
	printf("addr: %s\n", str);

	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		exit(2);
	}

	listen(listener, 10);
	while(1)
	{
		sock = accept(listener, NULL, NULL);
		pthread_t thread;
		pthread_create(&thread, NULL, thr_func, (void*) sock);
	}
}
