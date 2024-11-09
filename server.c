#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define h_addr h_addr_list[0]

int main( int argc, char *argv[] )
{
	if(argc != 2)
	{
		fprintf(stderr, "usage %s <port> \n", argv[0]);
        exit(0);
	}
	
	int sockfd, newsockfd ,clilen,tot;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	int portno = atoi(argv[1]);
	
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0); //Δημιουργία ενός socket με την βοήθεια της 'socket' funtion
	
	if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}
	
	
	bzero((char *) &serv_addr, sizeof(serv_addr)); // Αρχικοποίηση του socket

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // Δέσμεύει την διεύθυνση του server μέσω της bind  
	{
		perror("ERROR on binding");
		exit(1);
	}
	
	listen(sockfd,5);          // Ο  server περιμένει για την συνδέση με τον  client  
	clilen = sizeof(cli_addr);
	
	
	while(1)
	{
        
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); // Έδω ο server αποδέχεται την σύνδεση με τον client
        
        if (newsockfd==-1) 
        {
            perror("Accept");
            continue;
        }
        
        FILE* fp = fopen( "recv.txt", "w");
        tot=0;
        
        if(fp != NULL)  // Ο server αντιγράφει τα περιεχόμενα του αρχείο "send.txt" στο αρχείο "recv.txt" 
        {
            while( (n = recv(newsockfd, buffer, 255,0))> 0 ) 
            {
                tot+=n;
                fwrite(buffer, 1, n, fp);
            }

            printf("Received byte: %d\n",tot);
            
            if (n<0)
           	{
               perror("Receiving");
			}
			
            fclose(fp);
            
        } 
        else 
        {
            perror("File");
        }
        
        close(newsockfd);
    }
	
	return 0;
}
	
	
	
