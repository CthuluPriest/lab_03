#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



int main()
{
	int listener;
	struct sockaddr_in addr;
	
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
	while(1)
	{
		sock = accept(listener, NULL, NULL);
	}
}
