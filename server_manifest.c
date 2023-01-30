#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    int file_size;

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    // Set socket address
    memset(&server_addr, 0, sizeof(server_addr)); // clear values
    server_addr.sin_family = AF_INET; // ip-family set
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip-addr set
    server_addr.sin_port = htons(1234); // port address set

    // Bind the socket to the address
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("listen");
        return 1;
    }

    // Accept a connection from a client
    int client_addr_len = sizeof(client_addr);
    if ((client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_len)) < 0) {
        perror("accept");
        return 1;
    }

    // Open the .m3u8 file to be sent
    FILE *file = fopen("master.m3u8", "rb");
    if (!file) {
        perror("fopen");
        return 1;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file); // SEEK_BEG

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

    // Close the file and the socket
    fclose(file);
    close(client_socket);
    close(server_socket);

    return 0;
}
