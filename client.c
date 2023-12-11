// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#define PORT 8080
#define BUFFERSIZE 1024


int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "I want to download a file from the server :P ";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); // to make sure the struct is empty. Essentially sets sin_zero as 0
                                                // which is meant to be, and rest is defined below

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converts an IP address in numbers-and-dots notation into either a
    // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
    {
        printf("\nConnection Failed \n");
        return -1;
    }


    send(sock , hello , strlen(hello) , 0 );  // send the message.
    printf("Client: I just sent a request to the server to download a file!\n");
    valread = read( sock , buffer, 1024);  // receive message back from server, into the buffer
    printf("Server :%s\n",buffer );
    char file_name[BUFFERSIZE];
    file_name[0] = 0;
    scanf("%s",file_name);
    printf("Client: Server is requested to download %s file\n",file_name);
    send(sock,file_name,strlen(file_name),0);
    char buffer2[1024] = "";
    valread = read(sock,buffer2,1024);
    printf("Server : %s\n",buffer2);
    char compare[1024] = "Sorry! The file could not be found!\n";
    if(strcmp(buffer2,compare)==0)
      return 0;;

    char buffer3[1024]="";
    int fildes = open(file_name,O_CREAT | O_RDWR);
    while((valread = read(sock,buffer3,1024))>0)
    {
      int temp = write(fildes,buffer3,valread);
      if(temp<1024)
      {
        printf("The file has been successfully downloaded\n");
        break;
      }
    }
   close(fildes);
    return 0;
}
