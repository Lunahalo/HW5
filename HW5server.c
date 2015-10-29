#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <sys/unistd.h>
#include <sys/wait.h>

#include "Socket.h"


void printFromClient(void);

const int MAX_LINE_SIZE = 1024;
const int myPort = 16992;

ServerSocket welcome_socket;
Socket connect_socket;


int main(int argc, char* argv[]) {
    bool forever = true;
    int child_status;
    pid_t spid, term_pid;

    //welcome_socket = ServerSocket_new(atoi(argv[1]));
    welcome_socket = ServerSocket_new(myPort);
    if (welcome_socket < 0) {
        printf("Failed new server socket\n");
        exit(-1);
    }
    while(forever) {
        connect_socket = ServerSocket_accept(welcome_socket);
        if(connect_socket < 0) {
            printf("Failled to accept server socket\n");
            exit(-1);
        }
        spid = fork();
        if(spid == -1) {
            perror("There was an error with the server fork.");
            exit(-1);
        }
        if(spid == 0) {
            /*In child process*/
            printFromClient();
            Socket_close(connect_socket);
            exit(0);
        }
        else {
            /*In parent process*/
            Socket_close(connect_socket);
            term_pid = waitpid(-1, &child_status, WNOHANG);
            /*TO-DO check the term_pid for -1 indicating an error*/
        }
    }
}

void printFromClient(void) {
    int i;
    int intValOfChar;
    int returnValSocket_putc;
    int lineLength = 0;
    bool forever = true;
    char inputLine[MAX_LINE_SIZE];
    char outputLine[MAX_LINE_SIZE];

    Socket_close(welcome_socket);
    while(forever) {
        for (i = 0; i < MAX_LINE_SIZE; ++i) {
            intValOfChar = Socket_getc(connect_socket);
            if(intValOfChar == EOF) {
                printf("Socket_getc EOF or error\n");
                return;
            }
            else {
                if(intValOfChar == '\0') {
                    inputLine[i] = (char) intValOfChar;
                    break;
                }
                inputLine[i] = (char) intValOfChar;
            }
        }
        if(i == MAX_LINE_SIZE) inputLine[i-1] = '\0';
        /* I now have all the data sent by the client contained in inputLine, time
         * to send it back.*/
        printf("This was recieved from the client: %s", inputLine);
        char returnText[] = "Data was recieved by the server\n";
        lineLength = strlen(returnText) +1;
        for (i = 0; i < lineLength; ++i) {
            intValOfChar = returnText[i];
            returnValSocket_putc = Socket_putc(intValOfChar, connect_socket);
            if(returnValSocket_putc == EOF) {
                printf("Socket_putc EOF or error\n");
                return;
            }
        }
    }
return;
}