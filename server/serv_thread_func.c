#include "serv_thread_func.h"
#include "file_size.h"


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
	}
	
	//obtain file size
	lsize = getFileSize(pFile);

	
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
	printf("sent the last bit:\n");
	bytes_read = recv(p_sock, answer, answer_length,0);
	
	fclose (pFile);
	printf("closed file\n");
	send(p_sock, DONE, sizeof(DONE), 0);
	printf("sent done\n");;

	close(p_sock);	
	printf("closed socket\n");
}
