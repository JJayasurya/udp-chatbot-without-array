#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{

    char *ip="127.0.0.1";
    int port=5619;

    int socket_desc;
    struct sockaddr_in server_addr;
    char *server_message;
    char *client_message;
    int server_struct_length = sizeof(server_addr);
    
   
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_desc < 0)
    {
        perror("[-]Error while creating socket\n");
        return -1;
    }
    printf("[+]Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
 
    printf("...type \"exit\" to quit.....\a\n.");

     int size=100;
     client_message=(char*)calloc(size, sizeof(char));
     server_message=(char*)calloc(size, sizeof(char));
    
    while(1)
    {
    	// Clean buffers:
    	memset(server_message, '\0', size);
    	memset(client_message, '\0', size);
    
       
  
    	
        // Get input from the user:
    	printf("[+]client message: ");
    	fgets(client_message,size,stdin);
    	
        // Send the message to server:
    	if(sendto(socket_desc, client_message, strlen(client_message), 0,
        (struct sockaddr*)&server_addr, server_struct_length) < 0)
   	{
   		perror("[-]Unable to send message\n");
   		return -1;
    	}
     
        //client exited
    	if(strcmp(client_message, "exit\n") == 0)
    	{
        	printf("......client exited........\a\n");
		goto close_label;
    	}
    
    	// Receive the server's response:
    	if(recvfrom(socket_desc, server_message, 100, 0,
        (struct sockaddr*)&server_addr, &server_struct_length) < 0)
    	{
        	perror("[-]Error while receiving server's msg\a\n");
        	return -1;
    	}
    	printf("Server's response: %s\n", server_message);
    
        //Server exited
	if(strcmp(server_message, "exit\n") == 0)
    	{
        	printf("......Server exited........\a\n");
		goto close_label;
    	}
    }
   
    close_label:
    {
    	// Close the socket:
        free(server_message);
        free(client_message);
    	close(socket_desc);
    	printf("[-] disconnected from server \a");
    }   
    

    return 0;
}


