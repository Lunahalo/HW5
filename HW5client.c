/* On my honor, I have neither given or received unauthorized assistance on this program.
 * I have consulted with Steven Walter on the general approach to the solution.  I have also
 * used the files given by Professor Jeffay as a template and modeled my code after them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Socket.h"

const int MAX_LINE_SIZE = 1024;
const int myPort = 16992;

int main(int argc, char *argv[]) {
    char inputLine[MAX_LINE_SIZE], outputLine[MAX_LINE_SIZE];
    int i;
    int intValOfChar;
    int returnValSocket_putc;
    int lineLength = 0;
    if(argc < 2) {
        printf("No host and port\n");
        exit(-1);
    }

    Socket connect_socket;
    connect_socket = Socket_new(argv[1], myPort);
    /*Check that a possible host and port number is given*/
    //if (argc < 3) {

    printf("%% ");
    while(fgets(inputLine, sizeof(inputLine), stdin) != NULL) {
        lineLength = strlen(inputLine)+1;
        for(i = 0; i < lineLength; i++) {
            intValOfChar = inputLine[i];
            returnValSocket_putc = Socket_putc(intValOfChar, connect_socket);
            if(returnValSocket_putc  == EOF) {
                printf("Socket_putc EOF or error\n");
                Socket_close(connect_socket);
                exit(-1);
            }
        }
        for(i = 0; i < MAX_LINE_SIZE; i++) {
            intValOfChar = Socket_getc(connect_socket);
            if(intValOfChar == EOF) {
                printf("Socket_getc EOF or error\n");
                Socket_close(connect_socket);
                exit (-1);
            }
            else {
                outputLine[i] = intValOfChar;
                if(outputLine[i] == '\0') break;
            }
        }
        if(i == MAX_LINE_SIZE) outputLine[i-1] = '\0';
        printf("%s", outputLine);
    }
    Socket_close(connect_socket);
    exit(0);
}

char* parseHostName(char* inputHostname) {

}
