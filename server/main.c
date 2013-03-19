#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>

pthread_t thrs[10];

void * thr_func(int  p_sock){
	printf("lol\n");
	printf("%d",p_sock);
	printf("\n");	
}



int main(){
	int listener, sock;
	struct sockaddr_in addr;
	pthread_t pid;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
		perror("listener socket fail");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3561);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		exit(2);
	}

	listen(listener, 5);
	sock = 0;
//	while(1)
//	{
		sock = sock +1;
		pthread_create(&thrs[0], NULL, thr_func, (void*) sock);
		pthread_join(thrs[0], NULL);
		//sock = accept(listener, NULL, NULL);
//	}
}
