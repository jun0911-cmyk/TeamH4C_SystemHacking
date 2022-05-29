#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define HEADER_FMT "HTTP/1.1 %d %s\nContent-Length: %ld\nContent-Type: %s\n\n%s\n"

void error_handle(char *message);
void file_read(char buffer[]);
void setting_header(char* header, char* buffer, char html_buffer[]);

int main(int argc, char *argv[]) {
    int clnt_socket, valread, pid;
    int opt = 1;

    char buffer[1024] = {0};
    char header[1024] = {0};
    char html_buffer[1024];
    struct sockaddr_in address;

    int addresslen = sizeof(address);
    int serv_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (serv_socket == -1) {
        error_handle("socket create ERROR");
        return 0;
    }

    printf("\n[RUNNING...] serv_socket port : %s", argv[1]);
    printf("\n[RUNNING...] serv_socket create success");
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(atoi(argv[1]));

    if (bind(serv_socket, (struct sockaddr*) &address, sizeof(address)) == -1) {
        error_handle("bind ERROR");
        return 0;
    }

    printf("\n[RUNNING...] serv_socket binding success");

    if (listen(serv_socket, 3) < 0) {
        error_handle("listen ERROR");
        return 0;
    }

    printf("\n[RUNNING...] serv_socket listen success");

    while (1) {
        clnt_socket = accept(serv_socket, (struct sockaddr*)&address, (socklen_t *)&addresslen);
        
        if (clnt_socket == -1) {
            continue;
        }

        pid = fork();
        printf("%d\n", pid);

        if (pid == 0) {
            printf("\n[RUNNING...] clnt_socket accept success");
            close(serv_socket);
            read(clnt_socket, buffer, 1024);
            setting_header(header, buffer, html_buffer);
            write(clnt_socket, header, strlen(header));
        }
        
        if (pid != 0) {
            close(clnt_socket);
        } 
        
        if (pid == -1) {
            error_handle("fork ERROR");
        }
    }
}

void setting_header(char* header, char* buffer, char html_buffer[]) {
    char *method = strtok(buffer, " "); 
    char *uri = strtok(NULL, " ");
    char *status_text = "OK";
    char *ct_type = "text/html";
    file_read(html_buffer);
    sprintf(header, HEADER_FMT, 200, status_text, strlen(html_buffer), ct_type, html_buffer);
    printf("\n[SUCCESS] response header setting success");
}

void file_read(char buffer[]) {
    FILE *fp = fopen("index.html", "r");
    fgets(buffer, 1024, fp);
    printf("\n[SETTING...] html file reading success");
    fclose(fp);
}

void error_handle(char *message) {
    printf("\n%s", message);
}
