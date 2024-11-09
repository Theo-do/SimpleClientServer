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

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    FILE *fp=NULL;

    char buffer[256]; 
    
    if(argc != 3) // Έλενχος εάν δώθηκε σωστο hostname port
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //Δημιουργία του socket

    if(sockfd < 0) // Έλενχος εάν άνοιξε με επιτυχία το socket
    {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]); // Έλενχος έαν υπάρχει ο server
    if(server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0) //Συνδέση μεταξύ client και server
    {
        perror("ERROR connecting");
        exit(1);
    }
    
    
    if(fp==NULL) //Δημιουργία και εγγραφή στο αρχείου που θα αποσταλεί στον server.
    {
    	fp = fopen("send.txt", "wb");  
    	
   		if(fp == NULL)
    	{
       		perror("File");
       		return 2;
    	}
    	
		fprintf(fp,"hello world \n");
    	fclose(fp);
    }

	fp = fopen("send.txt", "rb"); 
    if(fp == NULL)
    {
        perror("File");
        return 2;
    }
	
    while( (n = fread(buffer, 1, sizeof(buffer), fp))>0 )  //Αποστολή του αρχείου στον server.
    {
        send(sockfd, buffer, n, 0);
    }

    fclose(fp);		// Κλείσιμο του αρχείου.
    


    return 0;
}


	 
									
    
    
    
