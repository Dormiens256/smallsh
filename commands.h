#ifndef SMALLSHCOMMANDS
#define SMALLSHCOMMANDS




struct command
{
    char* command;
    char** arguments;
    int argnum;
    char* input;
    char* output;
    int background; 
};

struct command *parsecommand(char *);
void cdcommand(struct command*);
void exitcommand(struct command*, int*);
void statuscommand(int);
void othercommand(struct command*, int**, int*, int*, int);
void harvestchildren(int** , int *, int* );

#endif