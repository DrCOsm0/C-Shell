#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80

int main(void) {
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;
    char line[MAX_LINE];
    int back_flag = 0;
    int status;
    int i = 0;
    
    while(should_run){
        printf("CSE222>");
        fflush(stdout);
        fgets(line, MAX_LINE, stdin);
        line[strcspn(line, "\nEOF")] = 0;
        args[i] = strtok(line, " ");

        
        //new command line
        if(strlen(line) <= 1){
            printf("\n");
            clearerr(stdin);
            continue;
        }

        //parse input command
        while(args[i] != NULL){
            i++;
            args[i] = strtok(NULL, " ");
        }

        //exit handeling
        if(strcmp(args[0], "exit") == 0) {
            break;
        }

        //normal execution
        if(strcmp(args[i-1], "&") != 0) {
            pid_t pid = fork();
            if(pid < 0) {
                fprintf(stderr,"Fork Error\n");
                return 1;
            } else if(pid == 0) {
                if(execvp(args[0],args) == -1){
                    printf("Command not Found\n");
                }
                exit(1);
            }
            else {
                waitpid(pid, &status, 0);
            }
        }

        //background execution
        if(strcmp(args[i-1], "&") == 0) {
            pid_t pid = fork();
            if(pid < 0) {
                printf("Fork Error\n");
                return 1;
            } else if(pid == 0) {
                args[i - 1] = NULL;
                if(execvp(args[0],args) == -1){
                    printf("Command not Found\n");
                }
                exit(1);
            }
        }
    }
    return 0;
}
