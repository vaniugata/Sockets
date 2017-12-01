#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <iostream>

int main( int argc, char** args ){
	int connection, transfer, port_no;
	socklen_t client_len;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	if( argc < 2 ){
		perror("No port provided.\n");
		exit(1);
	}
	//create connection socket
	connection = socket(AF_INET, SOCK_STREAM, 0);
	if( connection < 0 ) std::perror("Error opening socket!");

	bzero( (char*) &serv_addr, sizeof(serv_addr) );	
	port_no = atoi(args[1]); 

	//# bind server 
	serv_addr.sin_family = AF_INET;
	// set current host ip
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//convert port from int to string
	serv_addr.sin_port = htons( port_no ); //host short -> string

	if( bind(connection, (struct sockaddr*)& serv_addr, sizeof(serv_addr) ) < 0 ){
		perror("Error in binding");
	}
	listen( connection, 5 );
	client_len = sizeof(cli_addr);
	transfer = accept( connection, (struct sockaddr*) &cli_addr, &client_len );
	if( transfer < 0 ){
		perror("Error no connection established!");
	}
	printf("server: recieve connection from: %s with port: %d\n", 
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port) );
	
	printf("\nEnter message to client: ");
	bzero( buffer, sizeof(buffer) );
	fgets( buffer, sizeof(buffer)-1, stdin );
	send( transfer, buffer, sizeof(buffer), 0 );
	bzero(buffer, sizeof(buffer) );

	if( read( transfer, buffer, sizeof(buffer)-1 ) < 0 ){
		perror("Error reading from socket!\n");
	}
	printf( "client message: %s\n", buffer );
	bzero( buffer, sizeof(buffer) );
	
	printf("Session closed.\n");
	close( transfer );
	close( connection );
	return 0;
}
