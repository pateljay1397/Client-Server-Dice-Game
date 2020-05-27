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

void servicePlayers(int, int);

int main(int argc, char *argv[]){// E.g. 3: Server
int sd, portNumber, Player_1, player_2;
socklen_t len;
struct sockaddr_in servAdd;

if(argc !=2){
	fprintf(stderr, "call model: %s <port#>\n",argv[0]);
	exit(0);
}
//create a socket
if((sd = socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Could not create socket\n");
exit(1);
}
// Assigning Ip and Port
servAdd.sin_family = AF_INET;
servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
sscanf(argv[1], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);

//Binding the server socket with structure
if(bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd))<0){
	perror("Fail to Bind: \n");
	exit(1);
}
if(listen(sd, 3)){
	perror("Fail to Listen: \n");
}
printf("Binding is done \n Waiting for Players \n");

while(1){
	// Waiting For First Player
	player_1=accept(sd,(struct sockaddr*)NULL,NULL);
	Printf("First Player is Connected: \n");
	//Waiting For Second Player
	player_2=accept(sd,(struct sockaddr*)NULL,NULL);
	Printf("Second Player is Connected: \n");
	// Creating Child Process
	if(!fork()){
		servicePlayers(player_1,player_2);
	}
	close(player_1);
	close(player_2);
}
return 0;
}

void servicePlayers(int player_1,player_2){
	int final_score[2]={0}, received_score, maximum_score=100;
	
	char player_m[255],p1[255],p2[255];
	
	//Reading Player_1 Name
	if(!read(player_1,p1,100)){
		close(player_1);
		fprintf(stderr, "Player_1 data not received: \n");
		exit(0);
	}	
	//Reading Player_2 Name
	if(!read(player_2, p2, 100)){
		close(player_2);
		fprintf(stderr, "Player_2 data not received: \n");
		exit(0);
	}
	
	while(1){
		
		//Notify player_1 to play game
		strcpy(player_m,"You can start game: \n");
		write(player_1, player_m, strlen(player_m)+1);
		
		//Reading Player_1 score
		if(!read(player_1, player_m, 255)){
			close(player_1);
			fprintf(stderr,"player_1 data not received: \n");
			exit(0);
		}
		// Type Conversion of Score
		sscanf(player_m,"%d",&received_score);
		final_score[0] += received_score;
		printf("%s s Total Score is : %d\n",p1,final_score[0]);
		sleep(3);
		
		
		//Notify player_2 to play game
		strcpy(player_m,"You can start game: \n");
		write(player_2, player_m, strlen(player_m)+1);
		
		//Reading Player_2 score
		if(!read(player_2, player_m, 255)){
			close(player_2);
			fprintf(stderr,"player_2 data not received: \n");
			exit(0);
		}
		// Type Conversion of Score
		sscanf(player_m,"%d",&received_score);
		final_score[1] += received_score;
		printf("%s s Total Score is : %d\n",p2,final_score[1]);
		sleep(3);
		
		if(final_score[0] >= maximum_score){
			strcpy(player_m, "Game is over: You won the Game: \n");
			write(player_1, player_m, strlen(player_m)+1);			
			
			strcpy(player_m, "Game is over: You lost the Game: \n");
			write(player_2, player_m, strlen(player_m)+1);
			
			close(player_1);
			close(player_2);
			
			printf("\n %s Won the game",p1);			
			printf("\n Waiting for players to connect for new game: \n");
			exit(0);
		}
		if(final_score[1] >= maximum_score){
			strcpy(player_m, "Game is over: You won the Game: \n");
			write(player_2, player_m, strlen(player_m)+1);			
			
			strcpy(player_m, "Game is over: You lost the Game: \n");
			write(player_1, player_m, strlen(player_m)+1);
			
			close(player_1);
			close(player_2);
			
			printf("\n %s Won the game",p2);			
			printf("\n Waiting for players to connect for new game: \n");
			exit(0);
		}
	}
}
