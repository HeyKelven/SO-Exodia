#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 

#define VEN_DELIM_TOK " \t\r\n\a"
#define VEN_SIZE_TOK 32

//Teste para utilização de cores no terminal
#define COLOR_BLUE "\033[22;34m"
#define COLOR_GREEN "\033[22;32m"
#define COLOR_YELLOW "\033[22;33m"
#define COLOR_RED "\033[22;31m"
#define COLOR_CYAN "\033[22;36m"
#define COLOR_WHITE "\033[22;37m"
#define COLOR_RESET "\033[0m"

void runshell(void);
int ven_mud (char **);
int ven_aju (char **);
int ven_dir (char **);
int ven_lim (char **);
int ven_about (char **);
int ven_sr (char **);
char *ven_readline(void);
char **ven_splitline(char *);
int ven_laucher(char **);
int ven_exec(char **);

//os comandos que devem ser chamados
//é importante a ordem bater com lista de endereços logo depois

char *ven_strbuiltin[] = {
    "mudar",
    "ajuda",
    "dir",
    "limpar",
    "sobre",
    "sair"
};

int (*ven_fbuiltin[]) (char **) = {
    &ven_mud,
    &ven_aju,
    &ven_dir,
    &ven_lim,
    &ven_about,
    &ven_sr
};

int main (){
    runshell();

}

//retorna a quantidade de comandos criados
int ven_num_builtins(){
    return sizeof(ven_fbuiltin) / sizeof (char *);
}

char *ven_readline(void){
    char *writed_line = NULL;
    ssize_t needspace = 0;
    getline(&writed_line, &needspace, stdin);
    return writed_line;
}

char **ven_splitline(char *line){
    int linesize = VEN_SIZE_TOK, pos = 0;
    char **comm = malloc (linesize * sizeof(char *));
    char *part;

    if (!comm){
        fprintf(stderr, "ERROR.: We could not use the malloc funtion!\n");
        exit(EXIT_FAILURE);
    }

    part = strtok(line, VEN_DELIM_TOK);
    while (part != NULL){
        comm[pos] = part;
        pos++;

        if(pos >= linesize){
            linesize += VEN_SIZE_TOK;
            comm = realloc (comm, linesize * sizeof(char *));
        
            if(!comm){
                fprintf(stderr, "ERROR.: We could not use the realloc funtion!\n");
                exit(EXIT_FAILURE);
            }
        }
    part = strtok (NULL, VEN_DELIM_TOK);
    }
    comm[pos] = NULL;
    return comm;
}

int ven_laucher(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        if(execvp(args[0], args) == -1) perror("Error");
        exit(EXIT_FAILURE);

    } else if(pid < 0) perror("Error");

    else {
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int ven_mud (char **args){
    if(args[1] == NULL) fprintf (stderr, "Error.: We need the directory to continue!\n");
    else{
        if(chdir(args[1]) != 0) perror("Error");
    }
    return 1;
}

int ven_aju(char **args){    
    fprintf(stdout, "Ajuda:\n");
    fprintf(stdout, "Os seguintes comandos sao válidos!\n\n");

    int i;
    for(i = 0; i < ven_num_builtins(); i++)
        fprintf(stdout, "\t->%s\n", ven_strbuiltin[i]);
            
    fprintf(stdout, "\nOÉ nóis catchorro!\n");
    return 1;
}

int ven_lim(char **args){
    fprintf(stdout,"\033[H\033[J");
    return 71;
}


int ven_sr(char **args){
    ven_lim(args);
    fprintf(stdout, "Fechando Shellven!\n Até breve!\n");
    return 0;
}

int ven_dir(char **args){
    char cpath[1024];
    getcwd(cpath, sizeof(cpath));
    fprintf(stdout, "Diretório: %s\n", cpath);
}

int ven_exec(char **args){
    int  i;

    if(args[0] == NULL) return 1;

    for (i = 0; i < ven_num_builtins(); i++){
        if(strcmp(args[0], ven_strbuiltin[i]) == 0)
        return (*ven_fbuiltin[i])(args);
    }

    return ven_laucher(args);
}

int ven_about(char **args){
    printf(COLOR_BLUE "Shellven\tShellven\n" COLOR_RESET);
    printf(COLOR_GREEN"Shellven       Shellven\n"COLOR_RESET);
    printf(COLOR_RED"Shellven      Shellven\n"COLOR_RESET);
    printf(COLOR_CYAN"Shellven     Shellven\n"COLOR_RESET);
    printf(COLOR_WHITE"Shellven    Shellven\n"COLOR_RESET);
    printf(COLOR_BLUE"Shellven   Shellven\n"COLOR_RESET);
    printf(COLOR_CYAN"Shellven  Shellven\n"COLOR_RESET);
    printf(COLOR_RED"Shellven Shellven\n"COLOR_RESET);
    printf(COLOR_YELLOW"ShellvenShellven\n"COLOR_RESET);
    printf(COLOR_YELLOW"ShellvenShellven\n"COLOR_RESET);
    printf(COLOR_RED"Shellven Shellven\n"COLOR_RESET);
    printf(COLOR_CYAN"Shellven  Shellven\n"COLOR_RESET);
    printf(COLOR_BLUE"Shellven   Shellven\n"COLOR_RESET);
    printf(COLOR_WHITE"Shellven    Shellven\n"COLOR_RESET);
    printf(COLOR_CYAN"Shellven     Shellven\n"COLOR_RESET);
    printf(COLOR_RED"Shellven      Shellven\n"COLOR_RESET);
    printf(COLOR_GREEN"Shellven       Shellven\n"COLOR_RESET);
    printf(COLOR_BLUE "Shellven\tShellven\n" COLOR_RESET);
}

void runshell(void){

    char *line, **args;
    int status;

    int i = 0,j;
    do {
        for(j = 0; j <= i; j++)
            fprintf(stdout, "-");
        printf(">");
        i > 15 ? i = 15 : i++;

        line = ven_readline();
        args = ven_splitline(line);
        status = ven_exec(args);

        if(status == 71)
            i = 0; //visual para mostrar como andam as execuções dos comandos

        free(line);
        free(args);
    } while(status);
}