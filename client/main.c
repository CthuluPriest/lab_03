#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>


char message[] = "Hello";
char OK[] = "OK";
char answer[128];
char buf[1024];
char path[1024];
FILE *pFile;

int main()
{
	void* vsize;
	int sock;
	int bytes_read;
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
	recv(sock, answer, 128,0);
	printf("connection established \n");
	printf("file path: ");

	scanf("%s", path);
	send(sock, path, sizeof(path),0);
	//recv(sock, buf, sizeof(buf),0);
	
//	fsize = (int) vsize;
//	read(fsize);

	pFile = fopen("recieved_file", "wb");
	if(pFile == NULL) {
		printf("\n Write error \n");	
		exit(3);
	}else{
		printf("opened the file for output\n");
		do{
			bytes_read = recv(sock, buf, 1024, 0);
			printf("recieved the message %s\n", buf);
			if(strcmp(buf, "done") == 0) break;
			fwrite(buf, 1, bytes_read, pFile);
			printf("wrote the buffer to file...\n");
			send(sock, OK, sizeof(OK), 0);
			printf("sent back OK\n");
		}while(bytes_read > 0);
	}

	fclose(pFile);
//	printf("%s\n", path);
	close(sock);

	return 0;
	
}
