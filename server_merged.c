#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments\n");
        return 1;
    }

    int server_socket, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int file_size;

    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Forcefully attaching socket to the port 8080
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    int len = strlen(argv[1]);
    if (len >= 3 && strcmp(argv[1] + len - 3, ".ts") == 0) {
        // Send TS file to client
        FILE *file = fopen(argv[1], "rb");
        if(file == NULL) {
            perror("File open failed");
            exit(EXIT_FAILURE);
        }
        while(1) {
            // Read data into buffer
            int bytesRead = fread(buffer, 1, 1024, file);
            // If read was success, send data
            if(bytesRead > 0) {
                send(client_socket, buffer, bytesRead, 0);
            }
            if (bytesRead < 1024) {
                if (feof(file)) {
                    printf("End of file\n");
                }
                if (ferror(file)) {
                    printf("Error reading\n");
                }
                break;
            }
        }
        fclose(file);
    } else if (len >= 5 && strcmp(argv[1] + len - 5, ".m3u8") == 0) {
        // Open the .m3u8 file to be sent
        FILE *file = fopen("example.m3u8", "rb");
        if (!file) {
            perror("fopen");
            return 1;
        }

        // Get the file size
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);

        // Send the file size to the client
        sprintf(buffer, "%d", file_size);
        send(client_socket, buffer, strlen(buffer), 0);

        // Send the file contents to the client
        int bytes_sent = 0;
        while (bytes_sent < file_size) {
            int n = fread(buffer, 1, sizeof(buffer), file);
            send(client_socket, buffer, n, 0);
            bytes_sent += n;
        }
        fclose(file);
    } else {
        printf("Unknown file type\n");
    }
    
    close(client_socket);
    close(server_socket);
    return 0;
}
