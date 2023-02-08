#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <signal.h>
#include "commands.h"
#include "math.h"

//Constant that changes depending on wether we are in foreground only mode or not.  
int FOREGROUNDONLY = 1;             

void parentsigtstp(int sig){
    FOREGROUNDONLY = abs(FOREGROUNDONLY - 1);                   //alternates the constant between 0 and 1
    clearerr(stdin);                                            //not quite sure where these need to be so I put them in a couple places and it fixed a problem
    if(FOREGROUNDONLY == 0){                                    //prints out the correct message for the mode you just entered. The fflushs were there while troubleshooting and I am worried everything will fall apart without them
        write(STDOUT_FILENO, "Entering foreground-only mode (& is now ignored)\n", 50);
        fflush(stdout);
        fflush(stdin);
    }else{
        write(STDOUT_FILENO, "Exiting foreground-only mode\n", 30);
        fflush(stdout);
        fflush(stdin);
    }
    signal(SIGTSTP, parentsigtstp);                             //because the signal function undoes itself after being called once I call it again here to reset it up. 
    clearerr(stdin);
    return;
}

int main(){

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, parentsigtstp);                             //set up the parent ot ignore sigint and react prperly to ctrl z

    int status = -5;
    size_t inputmax = 2049; 
    int* runningpids; 
    int pidnum = 0; 
    char* input = (char *)malloc(inputmax * sizeof(char));      //sets up variables to work with input and keep track of pids and such
    while(0==0){
        harvestchildren(&runningpids, &pidnum, &status);        //goes and reaps the zombified corpses of children
        memset(input, '\0', inputmax);
        
        clearerr(stdin);
        printf(":");
        clearerr(stdin);
        fflush(stdout);
        getline(&input, &inputmax, stdin);
        struct command* curcommand = parsecommand(input);       //parses the command into a struct
        if(curcommand == NULL){
            
        }else if(strcmp(curcommand -> command, "cd") == 0){                            //the fact that I cant do a switch case using a string easily makes me sad
            cdcommand(curcommand);
            input = getcwd(input, 80);
        }else if(strcmp(curcommand -> command, "exit")  == 0 ){
            exitcommand(curcommand, runningpids);
            free(input);
            return 0; 
        }else if(strcmp(curcommand -> command, "status")  == 0 ){
            statuscommand(status);
        }else {
            othercommand(curcommand, &runningpids ,&pidnum ,&status, FOREGROUNDONLY);
        }
    
    }
    free(input);    //unnecessary but it makes me happier for this to be here. 
    return 0;
}

