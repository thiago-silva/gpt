#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> //close

/*
struct sockaddr_in
  {
      __SOCKADDR_COMMON (sin_);
    in_port_t sin_port;           Port number.
    struct in_addr sin_addr;            Internet address. (uint32) 

        Pad to size of `struct sockaddr'. 
        unsigned char sin_zero[sizeof (struct sockaddr) -
	                           __SOCKADDR_COMMON_SIZE -
				                              sizeof (in_port_t) -
							                                 sizeof (struct in_addr)];
											   };

 */

int create_server_socket(int port) {
	int sock;
  int on = 1;
	struct sockaddr_in name;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		printf("server: could not create socket\n");
		exit(EXIT_FAILURE);
	}

  /* Enable address reuse */  
  if( -1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) )) {//see http://www-128.ibm.com/developerworks/linux/library/l-sockpit/
    printf("server: unable to set option\n");
    exit(EXIT_FAILURE);
  }

	name.sin_family     = AF_INET;
	name.sin_port       = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
		printf("server: could not bind: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	return sock;
}

int main() {
	int port = 5555;
	int serversock;
	int clientsock;
	struct sockaddr_in clientname;
	size_t size = 0;
  char buffer[500];
	int received;


  char* cmd_step_into = "step_into";
  char* cmd_step_over = "step_over";
  char* cmd_step_out =  "step_out";
  char* cmd_continue =  "continue";
  char* cmd_top =  "stop";

	serversock = create_server_socket(port);
	if(listen(serversock, 1) < 0) { //1: queue size for receiving connections
		printf("server: error listening\n");
		exit(EXIT_FAILURE);
	}
	
  printf("waiting...\n");
	clientsock = accept(serversock, (struct sockaddr*) &clientname, &size);
	printf("server: connection accepted, addr size: %d\n", size);

  while(1) {
    printf("waiting for data...\n");
    received = recv(clientsock, buffer, sizeof(buffer), 0);
    if(received < 0) {
      printf("error receiving\n");
      exit(EXIT_FAILURE);
    } else   if(received == 0) {
      printf("no data received\n");
      break;
    } else {
      printf("total pack: %d, data received: \n>>>\n%s\n<<<\n", received, buffer);
    }
    printf("\n\n");

    if(send(clientsock, cmd_step_into, sizeof(char)* (strlen(cmd_step_into)+1), 0) < 0) {//error
      printf("client:error sending data\n");
      break;
    }
  }

	shutdown(clientsock, SHUT_RDWR);
	shutdown(serversock, SHUT_RDWR); //CLOSE ALL	
	close(clientsock);
	close(serversock);
}
