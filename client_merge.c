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

    int socket_descr, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    int file_size;

    // Creating socket file descriptor
    if ((socket_descr = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(socket_descr, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1],"ts")==0) {
        // receive the file and save it to disk
        FILE *file = fopen("rec_video.ts", "w");
        if (file == NULL) {
            perror("Error creating file");
            exit(1);
        }
        int bytes_received;
        while ((bytes_received = recv(socket_descr, buffer, 1024, 0)) > 0) {
            fwrite(buffer, 1, bytes_received, file);
        }
        fclose(file);

    } else if(strcmp(argv[1],"m3u8")==0) {
        recv (socket_descr, buffer, sizeof (buffer), 0);
        sscanf (buffer, "%d", &file_size);
        // Open a local file to save the .m3u8 file
        FILE * file = fopen ("rec_manifest.m3u8", "wb");
        if (!file) {
            perror ("fopen");
            return 1; 
        }
        // Receive the file contents from the server
        int bytes_received = 0;
        while (bytes_received < file_size) {
            int n = recv (socket_descr, buffer, sizeof (buffer), 0);
            fwrite (buffer, 1, n, file);
            bytes_received += n;
        }
        fclose(file);
    } else {
        printf("Unknown file type\n");
    }
    close (socket_descr);
    return 0;
}
