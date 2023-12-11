#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define BUFFERSIZE 1024

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;  // sockaddr_in - references elements of the socket address. "in" for internet
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *request_input = "Please enter the file to be downloaded:  ";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //This is to lose the pesky "Address already in use" error message
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    //                                               &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    // {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    address.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc.
    address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address - listens from all interfaces.
    address.sin_port = htons( PORT );    // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Port bind is done. You want to wait for incoming connections and handle them in some way.
    // The process is two step: first you listen(), then you accept()
    if (listen(server_fd, 3) < 0) // 3 is the maximum size of queue - connections you haven't accepted
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
while(1)
{
    // returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
    printf("hello!!  Server is up and running...\n");


    char file_name[BUFFERSIZE]="";

    read(new_socket,file_name,1024);
    printf("Server: The client has requested to download %s file\n",file_name);

    int rc_fd, dst_fd, n, err;
    FILE * src_fd;

    src_fd = fopen(file_name, "r");
    // if(src_fd == NULL)
    // {
    //   char *no_file="Sorry! File not Found";
    //   printf("Server: Sorry the file could not be found\n");
    //   send(new_socket , no_file , strlen(no_file) , 0 );  // use sendto() and recvfrom() for DGRAM
    //   printf("Message sent\n");
    //   continue;
    // }
  //  dst_fd = open(dest, O_CREAT | O_WRONLY);
  send(new_socket,request_input,strlen(request_input),0);

    printf("About to enter the loop...\n");
    while (1) {
        err = fread(buffer,1,1024,src_fd);
        if (err == -1) {
            printf("Error reading file.\n");
            exit(1);
        }
        printf("buffer is : %s\n",buffer);
        if(err>0)
        {
          printf("Sending Data....\n");
          send(new_socket,buffer,sizeof(buffer),0);
          printf("data sent...");
        }

        if(err<1024)
        {
          if(feof(src_fd))
            printf("End of FIle...\n");
          if(ferror(src_fd))
            printf("error reading\n");
          break;
        }

    }

  //  printf("Yeay! The file has been downloaded\n");
    break;
}
  close(new_socket);
    return 0;
}
