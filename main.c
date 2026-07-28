#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>

#define MAX_LINE 1024
#define MAX_ARGS 100

void signal_handler(int sig)
{
    printf("\nUtilisez 'exit' pour quitter le mini-shell\n");
    printf("mini-shell >>> ");
    fflush(stdout);
}

void afficher_prompt()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("\033[1;32m%s\033[0m >>> ", cwd);
    else
        printf("mini-shell >>> ");
    fflush(stdout);
}

void parse_command(char *cmd, char **args)
{
    int i = 0;
    char *token = strtok(cmd, " ");
    while(token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void execute_command(char **args)
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
    }
    else if(pid == 0)
    {
        execvp(args[0], args);
        perror("Commande inconnue");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }
}

void execute_pipe(char **cmd1, char **cmd2)
{
    int fd[2];
    pid_t pid1, pid2;
    if(pipe(fd) == -1)
    {
        perror("pipe");
        return;
    }
    pid1 = fork();
    if(pid1 == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(cmd1[0], cmd1);
        perror("Erreur commande 1");
        exit(EXIT_FAILURE);
    }
  
    pid2 = fork();
  
    if(pid2 == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        execvp(cmd2[0], cmd2);
        perror("Erreur commande 2");
        exit(EXIT_FAILURE);
    }
  
    close(fd[0]);
    close(fd[1]);
  
    wait(NULL);
    wait(NULL);
}

int main()
{

    char cmd[MAX_LINE];
    char *args[MAX_ARGS];

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, SIG_IGN);
  
    while(1)
    {

        afficher_prompt();
      
        if(fgets(cmd, sizeof(cmd), stdin)==NULL)
            break;
      
        cmd[strcspn(cmd,"\n")]='\0';
      
        if(strlen(cmd)==0)
            continue;
      
        char *pipe_pos = strchr(cmd,'|');
      
        if(pipe_pos != NULL)
        {
            *pipe_pos='\0';
            char *cmd1[MAX_ARGS];
            char *cmd2[MAX_ARGS];
            parse_command(cmd,cmd1);
            parse_command(pipe_pos+1,cmd2);
            execute_pipe(cmd1,cmd2);
            continue;
        }
    
        parse_command(cmd,args);

        if(args[0]==NULL)
            continue;
      
        if(strcmp(args[0],"exit")==0)
            break;

        if(strcmp(args[0],"cd")==0)
        {
            if(args[1]==NULL)
                printf("Usage : cd <dossier>\n");
            else if(chdir(args[1])!=0)
                perror("cd");
            continue;
        }

        execute_command(args);
    }
    printf("\nAu revoir !\n");
    return 0;
}
