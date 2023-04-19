#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define BUFFER_SIZE 25

int main()
{
    int pipefd[2]; // file descriptors for the pipe 
    pid_t pid; 
    char buffer[BUFFER_SIZE]; // buffer for the message

    if (pipe(pipefd) == -1) { // create the pipe and check for errors
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork(); // create a child process
   // if fork failed
    if (pid == -1) { 
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // child process
      //get a msg from user
        char msg[BUFFER_SIZE];
        printf("Enter a message: ");
        fgets(msg, BUFFER_SIZE, stdin);
        

        close(pipefd[0]); // close the read end of the pipe
        write(pipefd[1], msg, strlen(msg)); // send the message through the write end of the pipe
        close(pipefd[1]); // close the write end of the pipe
        exit(EXIT_SUCCESS); 
    }
    else 
    { // parent process
        close(pipefd[1]); // close the write end of the pipe
        read(pipefd[0], buffer, BUFFER_SIZE); // read the message from the read end of the pipe
        int index = strcspn(buffer, "\n");
        buffer[index] = '\0';

        printf("Received message from child: %s\n", buffer);
        close(pipefd[0]); // close the read end of the pipe
        exit(EXIT_SUCCESS);
    }
}



