#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>


char message[] = "Hello";
char buf[sizeof(message)];
char path[1024];
FILE *stream;

int main()
{
	void* vsize;
	int sock;
	int fsize;
	struct sockaddr_in addr;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0)
	{
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3561);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{	
		perror("connect");
		exit(2);	
	}

	send(sock, message, sizeof(message),0);
	recv(sock, buf, sizeof(message),0);
	printf("connection established \n");
	printf("file path: ");

	scanf("%s", path);
	send(sock, path, sizeof(path),0);
	recv(sock, buf, sizeof(buf),0);
	
//	fsize = (int) vsize;
	read(fsize);

	if((stream = freopen("file.in", "w", stdout)) == NULL) {
		printf("\n Write error \n");	
		exit(3);
	}
//	printf("%s\n", path);
	close(sock);

	return 0;
	
}
