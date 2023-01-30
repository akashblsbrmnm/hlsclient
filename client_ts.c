#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
int sock_fd, valread;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};
char *file_name = "rec_video.ts";
// Creating socket file descriptor
if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
}

serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(8080);
serv_addr.sin_addr.s_addr = INADDR_ANY;

// Connect to the server
if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connection failed");
    exit(EXIT_FAILURE);
}

// Receive and save the file
FILE *fp = fopen(file_name, "wb");
if(fp == NULL) {
    perror("File open failed");
    exit(EXIT_FAILURE);
}
while(1) {
    valread = read(sock_fd, buffer, 1024);
    if(valread > 0) {
        fwrite(buffer, 1, valread, fp);
    }
    if (valread < 1024) {
        if (feof(fp)) {
            printf("End of file\n");
        }
        if (ferror(fp)) {
            printf("Error reading\n");
        }
        break;
    }
}
printf("File received and saved\n");
return 0;
}
