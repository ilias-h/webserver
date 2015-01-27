
	printf("\n%s\n",string);
	exit(0);
}

int main(int argc,char *argv[]){

	int s,b,l,fd,sa,bytes,on=1;
	char buf[BUF_SIZE];
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

		sa = accepts(s,0,0);
		if(sa<0)
			fatal("Accept failed");

		read(sa,buf,BUF_SIZE);
	

		fd = open(buf,O_RDONLY);
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
