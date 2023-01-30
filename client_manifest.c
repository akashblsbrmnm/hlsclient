#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int file_size;

    // Create a socket
    if((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }
    // Set socket address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(1234);

    // Connect to the server
    if (connect(socket_desc, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    // Read the file size from the server
    recv(socket_desc, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%d", &file_size);

    // Open a local file to save the .m3u8 file
    FILE *file = fopen("received_file.m3u8", "wb");
    if (!file) {
        perror("fopen");
        return 1;
    }

    // Receive the file contents from the server
    int bytes_received = 0;
    while (bytes_received < file_size) {
        int n = recv(socket_desc, buffer, sizeof(buffer), 0);
        fwrite(buffer, 1, n, file);
        bytes_received += n;
    }

    // Close the file and the socket
    fclose(file);
    close(socket_desc);

    return 0;
}
