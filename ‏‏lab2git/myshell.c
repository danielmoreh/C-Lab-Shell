#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>



#include "LineParser.h"

#define MAX_PATH_SIZE 4096 
#define MAX_INPUT_SIZE 2048


void execute(cmdLine *pCmdLine);
void suspendProcess(int pid);
void killProcess(int pid);
void wakeProcess(int pid);
int isDigit(char c);
int isNumber(char *str);



int main(int argc, char **argv) {
    int debug = 0;
    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
        debug = 1;
    }
    char input[MAX_INPUT_SIZE];
    cmdLine *line;

    // while not EOF (i was not told to do it but i think it is a good idea to do it)
    while(!feof(stdin)) {

        char cwd[MAX_PATH_SIZE]; 
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");  // print the error message to stderr
            exit(EXIT_FAILURE); // exit the program with an error code
        }
        // print the current working directory 
        printf("%s> ", cwd);
        // read a line from the standard input 

        fgets(input, MAX_INPUT_SIZE, stdin);

        if (debug) {
            fprintf(stderr, "PID: %d\n", getpid());
            fprintf(stderr, "Executing command: %s", input);
        }

        //if the input is "quit" then break the loop
        if (strcmp(input, "quit\n") == 0) {
            printf("exit gracefully \n");
            break;
        }
       
       //the parseCmdLines function is in the LineParser.c file 
       //the function returns a pointer to a cmdLine struct that was build from the input string
        line = parseCmdLines(input);

        //execute the command line
        execute(line);

        
        //free the memory allocated for the command line
        freeCmdLines(line);

    }

    return 0;
}


void execute(cmdLine *pCmdLine) {
      if (strcmp(pCmdLine->arguments[0], "suspend") == 0) {            
            //chceck each char in the string to see if it is a digit 
            // if not then print an error message and return 
            if(isNumber(pCmdLine->arguments[1])){
                 suspendProcess(atoi(pCmdLine->arguments[1]));
                 return ;
            }
        } 
        else if (strcmp(pCmdLine->arguments[0], "wake") == 0) {
            if(isNumber(pCmdLine->arguments[1])){
                wakeProcess(atoi(pCmdLine->arguments[1]));
                return ;
            }
        }
         else if (strcmp(pCmdLine->arguments[0], "kill") == 0) {
            if(isNumber(pCmdLine->arguments[1])){
                  killProcess(atoi(pCmdLine->arguments[1]));
                  return ;
           }
        } 
    pid_t pid = fork();

    if (pid == -1) { // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process

        // Redirect standard input if inputRedirect is not NULL
        // freopen returns NULL on failure 
        // and the original file descriptor on success 
        if (pCmdLine->inputRedirect != NULL) {
            FILE* input_file = freopen(pCmdLine->inputRedirect, "r", stdin);
            if (input_file == NULL) {
                perror("freopen");
                _exit(EXIT_FAILURE);
            }
        }

        // Redirect standard output if outputRedirect is not NULL
        // freopen returns NULL on failure
        // and the original file descriptor on success
        if (pCmdLine->outputRedirect != NULL) {
            FILE* output_file = freopen(pCmdLine->outputRedirect, "w", stdout);
            if (output_file == NULL) {
                perror("freopen");
                _exit(EXIT_FAILURE);
            }
        }

        int ret = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        
        if (ret == -1) {
            perror("execvp");
            _exit(EXIT_FAILURE);
        }
    } else { // Parent process
       
        if (pCmdLine->blocking) { //if the & flag is not set then wait for the child process to finish
            int status;
            waitpid(pid, &status, 0);
        }
}
}
// Suspend a running process (SIGTSTP).
void suspendProcess(int pid) {
    int result = kill(pid, SIGTSTP);
    if (result == 0) {
        printf("Process %d suspended\n", pid);
    } else {
        perror("Failed to suspend process");
    }
}

// Wake up a sleeping process (SIGCONT).
void wakeProcess(int pid) {
    int result = kill(pid, SIGCONT);
    if (result == 0) {
        printf("Process %d woke up\n", pid);
    } else {
        perror("Failed to wake up process");
    }
}

// Terminate a running/sleeping process.
void killProcess(int pid) {
    //print hello world
    int result = kill(pid, SIGKILL);
    if (result == 0) {
        printf("Process %d killed\n", pid);
    } else {
        perror("Failed to kill process");
    }
}
int isNumber(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (!isDigit(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}
int isDigit(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

