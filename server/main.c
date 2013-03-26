#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define buf_length 1024
#define answer_length 128
#define path_length 100

void * thr_func(void* arg){
	FILE* pFile;
	char *path;
	int p_sock = (int) arg;
	char buf[buf_length];
	char answer[answer_length];
	char OK[] = "OK"; 
	char BADVOODOO[] = "BADVOODOO";
	char DONE[] = "___done___";
	int bytes_read;
	long lsize;
	int i;
	int message_count;
	path = (char*) malloc(sizeof(char)*path_length);

	printf("server started\n");
	
	//exchanging hello message
	bytes_read = recv(p_sock, answer, answer_length,0);
		
	if(bytes_read < 0){
		printf("Couln\'t establish the connection\n");
		close(p_sock);
		exit(1);
	}
	send(p_sock, answer, bytes_read, 0);
	printf("sent hello\n");

	bytes_read = recv(p_sock, path, path_length,0);
	
	if(bytes_read < 0){
		printf("Couldn\'t recieve the file address\n");
		close(p_sock);
		exit(2);
	}

	printf("got address: %s\n", path);
	
	pFile = fopen(path, "rb");
	if(pFile == NULL) {
		perror("file");
		send(p_sock, BADVOODOO, sizeof(BADVOODOO), 0);
		return 0;	
		//exit(3);
	}
	
	//obtain file size
	fseek (pFile , 0 , SEEK_END);
	lsize = ftell (pFile);
	rewind (pFile);
	//calculate the number of messages expected
	message_count = lsize/buf_length;
	
	//starting file transmittion
	printf("Transmittion started...\n");
	for(i = 0; i<message_count; i++) 
	{
		//if smth went wrong free the resources and exit
		if ( fread (buf , 1, buf_length, pFile) <= 0 );
		{
			perror("fread");
			fclose(pFile);
			close(p_sock);
			free(path);
			exit(4);
		}
	
		//send message
		send(p_sock, buf, buf_length, 0);

		//wait for cliend to acknowledge that he got the message
		bytes_read = recv(p_sock, answer, answer_length,0);

		if(bytes_read < 0){
			perror("trouble with recv");
			fclose(pFile);
			close(p_sock);
			free(path);
			exit(5);	
		}
		
		if(strcmp(answer, OK) != 0){
			printf("Client refused a message\n");
			fclose(pFile);
			close(p_sock);
			free(path);
			exit(5);
		}
	}
	printf("Just a little more...\n");
	
	int leftover = lsize - message_count*buf_length;
	printf("leftover: %d\n", leftover);

	if ( fread (buf , 1, leftover, pFile) <= 0 ){
		perror("Reading trouble");
		fclose(pFile);
		close(p_sock);
		free(path);
		exit(6);
	}
	
	send(p_sock, buf, leftover, 0);
	printf("sent the last bit:%s\n", buf);
	bytes_read = recv(p_sock, answer, answer_length,0);
	
	fclose (pFile);
	printf("closed file\n");
	send(p_sock, DONE, sizeof(DONE), 0);
	printf("sent done\n");;

	close(p_sock);	
	printf("closed socket\n");
}



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
	sock = 0;
	while(1)
	{
		sock = accept(listener, NULL, NULL);
		pthread_t thread;
		pthread_create(&thread, NULL, thr_func, (void*) sock);
	}
}
