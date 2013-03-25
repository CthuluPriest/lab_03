#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <arpa/inet.h>

int thr_vacant[10];
char path[1024];

/*struct thr_args{
	int p_sock;
	int index;
};*/








void * thr_func(void* arg){
	FILE* pFile;
	int p_sock = (int) arg;
	char buf[1024];
	char answer[128];
	int bytes_read;
	long lsize;
	int i;
	int message_count;

	//args.p_sock = ((struct thr_args *)arg)->p_sock;
	//args.index = ((struct thr_args *)arg)->index;
	printf("started\n");
	
	//exchanging hello message
	bytes_read = recv(p_sock, answer, 128,0);
		
	if(bytes_read < 0){
		printf("Couln\'t establish the connection\n");
		close(p_sock);
		exit(3);
	}
	send(p_sock, answer, bytes_read, 0);
	printf("sent hello\n");

	bytes_read = recv(p_sock, path, 1024,0);
	
	if(bytes_read < 0){
		printf("Couldn\'t recieve the file address\n");
		close(p_sock);
		exit(3);
	}

	printf("got address: %s\n", path);
	
	pFile = fopen(path, "rb");
	if(pFile == NULL) {
		printf("\n Couldn\'t open the file \n");	
		exit(3);
	}
	
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lsize = ftell (pFile);
	rewind (pFile);
	message_count = lsize/1024;
	
	printf("Transmittion started...\n");
	for(i = 0; i<message_count; i++) 
	{
		printf("Checked feof\n");
		if ( fread (buf , 1, 1024, pFile) <= 0 );// break;
		{
			printf("fread failed\n");
			break;
		//	fclose(pFile);
		//	close(p_sock);
		//	exit(3);
		}
		printf("Filled the buffer...%s\n",buf);
		send(p_sock, buf, 1024, 0);
		printf("sent the package\n");
		bytes_read = recv(p_sock, answer, 128,0);
		printf("got the answer: %s\n", answer);
		if(bytes_read < 0){
			printf("The client refused a message\n");
			break;	
		}
	}
	printf("out the cycle\n");
	
	int leftover = lsize - message_count*1024;
	printf("leftover: %d\n", leftover);

	if ( fread (buf , 1, leftover, pFile) <= 0 ){
		printf("no leftover\n");
	}
	
	send(p_sock, buf, leftover, 0);
	bytes_read = recv(p_sock, answer, 128,0);
	
	fclose (pFile);
	printf("closed file\n");
	send(p_sock, "done", sizeof("done"), 0);
	printf("sent done\n");
	//send(args.p_sock,(void*) fsize,sizeof(int), 0);

	close(p_sock);	
	printf("closed socket\n");
}



int main(){
	int listener, sock;
	struct sockaddr_in addr;
	pthread_t pid;
	int i;

	for (i = 0; i<10; i++){
		thr_vacant[i] = 1;
	}	


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
	while(1)
	{
		//sock = sock +1;
		sock = accept(listener, NULL, NULL);
	//	struct thr_args *tmp;
	//	tmp = malloc(sizeof(*tmp));
	//	tmp->p_sock = sock;
		
		pthread_t thread;
		
	//	i = 0;
	//	printf("%d\n%d\n",tmp.p_sock, tmp.index);
		pthread_create(&thread, NULL, thr_func, (void*) sock);
		//sock = accept(listener, NULL, NULL);
	}
}
