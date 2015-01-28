#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SERVER_PORT 12345
#define BUF_SIZE 4096
#define QUEUE_SIZE 10

static void fatal(char * string){
	printf("\n%s\n",string);
	exit(0);
}

char t[10];
static char * parse(char * str, char c){
		int i=0;

		memset(&t,' ',sizeof(t));

		while(*str != c){
			t[i]=*str;
			str++;
			i++;
		}
	return t;
}

static int string_equal(char* str, char* str2, int size){
	int i = 0;
	while(i<size){
		if(*(str + i) != *(str2 + i))
			return 0;
		i++;
	}
	return 1;
}

int main(int argc,char *argv[]){

	int s,b,l,fd,sa,bytes,on=1;
	char buf[BUF_SIZE], * temp;
	struct sockaddr_in channel;

/* Address structure */
	memset(&channel,0,sizeof(channel));
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
	channel.sin_port = htons(SERVER_PORT);

/* Build socket and Bind */
	s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); /* Create socket */
	if(s<0)
		fatal("Failed to create socket!");

	setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char*)&on,sizeof(on));

	b = bind(s,(struct sockaddr *)&channel,sizeof(channel));
	if(b<0)
		fatal("Bind failed ");

	l = listen(s,QUEUE_SIZE);

	if(l<0)
		fatal("Listen Failed!");

/* Time to accept */

	while(1){
		printf("\nReady to take reuests\n");
		sa = accept(s,0,0);
		if(sa<0)
			fatal("Accept failed");

		read(sa,buf,BUF_SIZE);

		printf("\n%s\n", buf);

		printf("\n%s\n",parse(buf, ' '));

		if(string_equal(parse(buf, ' '),"GET",3)){
			printf("\n%s\n",parse(&buf[4],' '));
		}
					
				
		fd = open(parse(&buf[4],' '),O_RDONLY);
		if(fd<0)
			fatal("Open message failed!");

		while(1){
			bytes = read(fd,buf,BUF_SIZE);
			if(bytes <= 0)break;
			write(sa,buf,bytes);
		}
		close(fd);
		close(sa);
	}

}
