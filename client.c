#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int server, port_number, player_score;
	char server_msg[255], s_score;
	struct sockaddr_in serveraddress;
	
	if(argc != 4){
		printf("Call model:%s <IP> <Port#>\n",argv[0]);
		exit(0);
	}
	
	//Create a Socket
	if ((server=socket(AF_INET,SOCK_STREAM,0))<0){
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}
	
	//Assigning IP And Port
	
	servaddress.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servaddress.sin_port = htons((uint16_t)portNumber);
	
	//Converting IPV4 address from presentation to network format
	
	if(inet_pton(AF_INET, argv[3],&servaddress.sin_addr) < 0){
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	
	//Establishing the connection with server
	if(connect(server, (struct sockaddr *) &servaddress,sizeof(servaddress))<0){
		fprintf(stderr, "connection failed, exiting\n");
		exit(1);
	}
	
	printf("%s has successfully established connection with server: ", argv[1]);
	write(server, argv[1], strlen(argv[1])+1);
	
	while(1){
		//Reading Message From Server
		if (read(server, server_msg, 255)<0){
			fprintf(stderr, "read() error\n");
			exit(0);
		}
		//Player can roll dice
		if((strcmp(server_msg, "Now You can Play")==0)){
			printf("%s", server_msg);
			
			//Random Number Generations 
			player_score = (int)time(NULL)%7 + 1;
			
			//Before sending Player_score to server, Convert it from char to char array
			sprintf(s_score, %d , player_score);
			printf("\n%s's player obtained score : %s\n\n", argv[1],s_score);
			strcpy(server_msg, s_score);
			write(server, server_msg, strlen(server_msg)+1);
		}
		
		// Player after winning the game
		if((strcmp(server_msg, "Game is Over: You Won the game")==0)){
			printf("I Won the Game\n\n");
			close(server);
			exit(0);
		}
		
		// Player Lost the game
		if((strcmp(server_msg,"Game is over: You lost the game")==0)){
			printf("You Lost the Game\n\n");
			close(server);
			exit(0);
		}
	}
	return 0;
}
