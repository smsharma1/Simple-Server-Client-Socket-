#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 6060             /* port number */
#define CLEAR(x) memset(x,'\0',1024)
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char buffer1[1024] = {0};
    char filesize[10] = {0};

    char *line, *username1, *passwd1, *username, *passwd;
    FILE *fstream;
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        printf("socket failed");
        return -1;
    }
      
    // Forcefully attaching socket to the port 6060
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("setsockopt");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 6060
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("bind failed");
        return -1;
    }

    if (listen(server_fd, 5) < 0)
    {
        printf("listen");
        return -1;
    }

    while(1)
    {
      //accepting request from client
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
        (socklen_t*)&addrlen))<0)
      {
        printf("accept error");
        return -1;
      }
      	// child process because return value zero
        // For concurrancy 
      if (fork()==0)
    	{
        fstream = fopen("users.txt","r");

        if(fstream == NULL)
        {
          printf("\n file opening failed ");
          return -1 ;
        }
        CLEAR(buffer);
        valread = read(new_socket, buffer, 1024);
        username = strtok(buffer,":");
        passwd = strtok(NULL,":");
        int flag = 0;

        while((fgets(buffer1,sizeof(buffer1),fstream))!=NULL)
        {
        	  username1 = strtok(buffer1,",");
          	passwd1 = strtok(NULL,",");
        
        // checking is user is present 
	  		if((strcmp(username,username1)==0))
	  		{
          //checking if password of user is correct
	    		if(strcmp(passwd1,passwd)==0)
          {
            CLEAR(buffer1);
				    strcpy(buffer1,"Hello ");
					  strcat(buffer1,username);
	          send(new_socket , buffer1, 1024 , 0 );
	         	flag = 1;
					  break;
				  }
	  		}
	  		CLEAR(buffer1);
    	}
      fclose(fstream);
      //If user is not present or password is not correct
      if(flag == 0)
      {
        CLEAR(buffer);
        strcpy(buffer,"Authentication Failure!!!");
        send(new_socket , buffer, 1024 , 0 );
        close(new_socket);
	  		return 0;
      }
      CLEAR(buffer);
      valread = read(new_socket, buffer, 1024);
      //if file is present 
      if (fstream = fopen(buffer,"r"))
      {
        fseek(fstream, 0L, SEEK_END);
        sprintf(filesize, "%ld", ftell(fstream));
        fseek(fstream, 0L, SEEK_SET);
        CLEAR(buffer1);
        strcpy(buffer1,filesize);
        send(new_socket, buffer1, 1024, 0);
        while(fread(buffer1,1,1023,fstream) > 0)
        {
          send(new_socket, buffer1, 1023, 0);
        }
      }
      //if file is not present on system
      else
      {
        CLEAR(buffer1);
        strcpy(buffer1,"File Not Found");
        send(new_socket , buffer1 , 1024 , 0 );
      }
      return 0;
    }
	}
  return 0;
}
