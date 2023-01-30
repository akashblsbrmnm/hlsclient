#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *file_name = "master10.ts";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Send file to client
    FILE *fp = fopen(file_name, "rb");
    if(fp == NULL) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }
    while(1) {
        // Read data into buffer
        int bytesRead = fread(buffer, 1, 1024, fp);
        // If read was success, send data
        if(bytesRead > 0) {
            send(new_socket, buffer, bytesRead, 0);
        }
        if (bytesRead < 1024) {
            if (feof(fp)) {
                printf("End of file\n");
            }
            if (ferror(fp)) {
                printf("Error reading\n");
            }
            break;
        }
    }
    printf("File sent\n");
    return 0;
}
