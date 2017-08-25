#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 6060                 /* port number */
#define CLEAR(x) memset(x,'\0',1024)         /* function to clear buffer */
  
int main(int argc, char const *argv[])
{
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        printf("Please enter your credentials properly");
		return -1;
    }
    char* credentials = strtok(argv[1],"@");   /* extract username and password supplied from command line*/
    char* serverip = strtok(NULL,"@");         /* extract serverip in order to connect to server*/
   
    if (!credentials || !serverip)
    {
		printf("Please enter your credentials properly");
        return -1;
    }
    struct sockaddr_in address;      
    int sock = 0, valread,filesize = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char filename[100]={0};
    FILE *fstream;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, serverip, &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    CLEAR(buffer);
    strcpy(buffer,credentials);
    if (send(sock, buffer, 1024, 0) == -1) 
    {
		printf("Error sending credentials\n");
        close(sock);
		return -1;
    }
    CLEAR(buffer);
    valread = read(sock, buffer, 1024);
    if(strcmp(buffer,"Authentication Failure!!!")==0)
    {
		printf("%s\n",buffer);
        close(sock);
		return 0;
    }
    printf("Please enter the file name you wanna download:\n");
    scanf("%s",filename);
    CLEAR(buffer);
    strcpy(buffer,filename);
    if (send(sock, buffer, 1024, 0) == -1)
    {
        printf("Error sending filename\n");
        close(sock);
		return -1;
    }
    CLEAR(buffer);
    valread = read(sock, buffer, 1024);
    if(strcmp(buffer,"File Not Found")==0)
    {
		printf("%s\n",buffer);
		close(sock);
    	return 0;
    }
    filesize = atoi(buffer);
    fstream = fopen(filename, "w");
    while(filesize > 0)
    {
		valread = read(sock, buffer, 1023);
        if (filesize < 1023)
        {
            fwrite (buffer , sizeof(char), filesize, fstream);
        }
        else
        {
            fwrite (buffer , sizeof(char), 1023, fstream);
        }
		filesize-=strlen(buffer);
    } 
    close(sock);
    return 0;
}
