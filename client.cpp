#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <iostream>



int main(int argc, char** args){
	int connection, port_no;
	struct sockaddr_in serv_addr;
	struct hostent* server;
	char buffer[256];

	if( argc < 3){
		fprintf(stderr, "usage %s hostname port.", args[0] );
		_exit(0);
	}
	port_no = atoi(args[2]);
	connection = socket( AF_INET, SOCK_STREAM, 0 );
	if( connection < 0 ){
		perror("Error opening socket.\n");
		exit(0);
	}
	server = gethostbyname(args[1]);
	if( !server ){
		fprintf(stderr, "Error no such host.\n");
		exit(0);
	}
	bzero( (char*)& serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	bcopy( (char*) server->h_addr, (char*)& serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons( port_no );
	if( connect( connection, (struct sockaddr*)& serv_addr, sizeof(serv_addr) ) < 0 ){
		perror("Error establishing connection to server.\n");
		exit(0);
	}
	printf("client:  ");
	bzero( buffer, sizeof(buffer) );
	fgets( buffer, sizeof(buffer)-1, stdin);
	if( write( connection, buffer, strlen(buffer) ) < 0 ){
		perror("Error writing to socket.\n");
		exit(0);
	}
	if( read( connection, buffer, sizeof(buffer)-1 ) < 0 ){
		perror("Error: unable to read from socket.\n");
		exit(0);
	}
	printf("server message: %s\n", buffer );
	bzero( buffer, sizeof(buffer) );
	
	close( connection );
	printf("Session closed.\n");
	return 0;
}
