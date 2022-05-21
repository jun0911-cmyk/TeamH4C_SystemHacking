#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define HEADER_FMT "HTTP/1.1 %d %s\nContent-Length: %ld\nContent-Type: %s\n\n%s\n"

void error_handle(char *message);

int main(int argc, char *argv[]) {
    int clnt_socket, valread, pid;
    int opt = 1;

    char buffer[1024] = {0};
    char header[1024] = {0};
    char html_buffer[1024];
    char *test = "HTTP/1.1 200 OK";
    struct sockaddr_in address;

    int addresslen = sizeof(address);
    int socketCreate = socket(AF_INET, SOCK_STREAM, 0);

    if (socketCreate == -1) {
        error_handle("socket create ERROR");
        return 0;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(atoi(argv[1]));

    if (bind(socketCreate, (struct sockaddr*) &address, sizeof(address)) == -1) {
        error_handle("bind ERROR");
        return 0;
    }

    if (listen(socketCreate, 3) < 0) {
        error_handle("listen ERROR");
        return 0;
    }

    while (1) {
        clnt_socket = accept(socketCreate, (struct sockaddr*)&address, (socklen_t *)&addresslen);
        
        if (clnt_socket == -1) {
            continue;
        }

        pid = fork();
        printf("%d\n", pid);

        if (pid == 0) {
            close(socketCreate);
            read(clnt_socket, buffer, 1024);
            char *method = strtok(buffer, " "); 
            char *uri = strtok(NULL, " ");
            char *status_text = "OK";
            char *ct_type = "text/html";
            FILE *fp = fopen("index.html", "r");
            fgets(html_buffer, sizeof(html_buffer), fp);
            sprintf(header, HEADER_FMT, 200, status_text, strlen(html_buffer), ct_type, html_buffer);
            printf("%s", header);
            write(clnt_socket, header, strlen(header));
            fclose(fp);
        }
        
        if (pid != 0) {
            close(clnt_socket);
        } 
        
        if (pid == -1) {
            error_handle("fork ERROR");
        }
    }
}

void error_handle(char *message) {
    printf("\n%s", message);
}
