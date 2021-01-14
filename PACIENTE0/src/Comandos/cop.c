/*Command Processor (CoP)*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cop.h>

#define SENTENCE_LENGTH 150
#define COMMAND_NAME 30
#define FUNCTION_NAME 30

/*Structs*/

struct _Ext_cmd{
char *cmd;
char *internal;
int n_msg;
char **msg;
};

struct _Int_cmd{
char *i_name;
cmdfun_type fct;
};

struct _CoP{
ext_cmd **e_lst; /* The list with the "normal" commands... */
int ext_no; /* ...and the number of elements in it */
ext_cmd *error; /* The "special" error command */
int_cmd **i_lst; /* List with associations between internal commands and functions */
int int_no; /* and the number of elements it contains (0 at the beginning) */
int int_max; /* The number of pointers i_lst that are actually allocated */
};

/*Prototypes of internal functions*/
static ext_cmd* _ext_src(CoP *c, char *name);
static char **_unpack_all(ext_cmd *e, char *obj);
static cmdfun_type _assoc_search(CoP *c, char *i_name) ;
char *strdup(const char *s); /*For some reason, it generates a compilation warning if i do not write this*/

/*Functions*/

/*CREATE FUCTION*/

void CoP_free(CoP * command){
    int y, z;

    /*Liberación de comandos externos*/
    for(y=0;y<command->ext_no;y++){
        if(command->e_lst[y]->n_msg>0){
            for(z=0;z<command->e_lst[y]->n_msg;z++){
                free(command->e_lst[y]->msg[z]);
            }
            free(command->e_lst[y]->msg);
        }
        free(command->e_lst[y]->cmd);
        free(command->e_lst[y]->internal);
        free(command->e_lst[y]);
    }
    free(command->e_lst);

    /*Liberación de comandos internos*/
   
    for(y=0;y<command->int_no;y++){
        free(command->i_lst[y]->i_name);
        free(command->i_lst[y]);
    }
    free(command->i_lst);

    /*Liberación de error*/
    for(y=0;y<command->error->n_msg;y++)
        free(command->error->msg[y]);
    free(command->error->msg);
    free(command->error->internal);
    free(command->error->cmd);
    free(command->error);

    free(command);
}


/*
Creates a CoP structure from commands in cmdfile
It could work unpredictably if cmdfile does not have the following structure:
OBLIGATORY_ONLY_ONE_WORD_COMENTARY
COMMAND_NAME
command_function
numer_of_sentences
sentence 1
...
sentence n
*/
CoP *CoP_create(FILE *cmdfile){
    CoP *command;
    char nothing[100], command_name[20], command_fun[20], sentence[SENTENCE_LENGTH];
    int x, y, num_sentence;

    if(cmdfile==NULL)
        return NULL;

    command = (CoP*) malloc(sizeof(CoP));
    if(command==NULL)
        return NULL;

    /*Comandos internos*/
    command->i_lst = (int_cmd**) malloc(sizeof(int_cmd*));
    if(command->i_lst==NULL){
        free(command);
        return NULL;
    }
    command->int_no=0;
    command->int_max=1;

    /*Lectura del número de comandos*/
    fscanf(cmdfile, "%d", &x);
    command->ext_no = x;

    command->e_lst = (ext_cmd**) malloc((command->ext_no)*sizeof(ext_cmd*));
    if(command->e_lst==NULL){
        free(command->i_lst);
        free(command);
        return NULL;
    }

    /*Sentencias correctas*/
    for(x=0;x<command->ext_no;x++){ 
        command->e_lst[x] = (ext_cmd*) malloc(sizeof(ext_cmd));
            if(command->e_lst[x]==NULL){
                CoP_free(command);
                return NULL;
            }
        fscanf(cmdfile,"%s\n%s\n%s\n%d\n", nothing, command_name, command_fun, &num_sentence);
        command->e_lst[x]->cmd = (char*)strdup(command_name);
        command->e_lst[x]->internal = (char*)strdup(command_fun);
        command->e_lst[x]->n_msg=num_sentence;

        if(command->e_lst[x]->n_msg>0){
            command->e_lst[x]->msg = (char**) malloc((command->e_lst[x]->n_msg)*sizeof(sentence));
                if(command->e_lst[x]->msg==NULL){
                    CoP_free(command);
                    return NULL;
                }

            for(y=0;y<command->e_lst[x]->n_msg;y++){
            fgets(sentence, SENTENCE_LENGTH, cmdfile);
            command->e_lst[x]->msg[y] = (char*)strdup(sentence);
            }
        }
    }

    /*Sentencia de error*/
    command->error = (ext_cmd*) malloc(sizeof(ext_cmd));
    if(command->e_lst==NULL){
        CoP_free(command);
        return NULL;
    }

    fscanf(cmdfile,"%s\n%s\n%s\n%d\n", nothing, command_name, command_fun, &num_sentence);
    command->error->cmd = (char*)strdup(command_name);
    command->error->internal = (char*)strdup(command_fun);
    command->error->n_msg=num_sentence;
    command->error->msg = (char**) malloc((command->error->n_msg)*sizeof(char*));
    if(command->error->msg==NULL){
        CoP_free(command);
        return NULL;
    }
    for(y=0;y<command->error->n_msg;y++){
        fgets(sentence, SENTENCE_LENGTH, cmdfile);
        command->error->msg[y] = (char*) strdup(sentence);
    }    

    return command;
}


/*
Local function: searches for an association given an internal name;
returns a pointer to the associated function, or NULL if the
internal name is not found.
*/
static cmdfun_type _assoc_search(CoP *c, char *i_name) {
    int i;
    for (i=0; i<c->int_no; i++) {
        if ((strcmp(i_name, c->i_lst[i]->i_name)==0)) {
            return c->i_lst[i]->fct;
        }
    }
    return NULL;
}


/*Creates a new int_cmd and adds it to the CoP if it does not exist yet
Arguments:
-CoP where the int_cmd will be added
-Internal name of the command
-Function to which this command is associated
Returns:
-Number of int_cmd in CoP
-In case of error (-1)
*/
int CoP_assoc(CoP *c, char *int_name, cmdfun_type cfun) {
    int_cmd *new_a;     

    if (_assoc_search(c, int_name) != NULL) {
        return -1; /* the internal command already existed */
    }

    if (c->int_no >= c->int_max-1) { /* list full: get more space */
        c->int_max += 10;
        c->i_lst = (int_cmd **) realloc(c->i_lst, (c->int_max)*sizeof(int_cmd**));
        if(c->i_lst==NULL)
            return -1;
    }
    
    new_a = (int_cmd *) malloc(sizeof(int_cmd));
    if(new_a==NULL)
        return -1;

    new_a->i_name = (char*) strdup(int_name);
    new_a->fct = cfun;
    c->i_lst[c->int_no] = new_a;
    c->int_no=c->int_no+1;

    return c->int_no;
}


/*Searches the ext_command with command name name in c*/
static ext_cmd* _ext_src(CoP *c, char *name) {
    int i;


    for(i=0;i<c->ext_no;i++){
        if(strcmp(c->e_lst[i]->cmd, name)==0){
            return c->e_lst[i];
        }
    }

    return NULL;
}


/*
Prepares the strings of an external command for execution: returns
an array of prepared strings with the starts removed and replaced
by the name of the predicate
*/
static char **_unpack_all(ext_cmd *e, char *obj) {
    int i;
    char **str=NULL;
    
    if(e->n_msg>0){
        str = (char **) malloc(e->n_msg*sizeof(char *));
        for (i=0; i<e->n_msg; i++)
            str[i] = _unpack(e->msg[i], obj);
    }
    
    return str;
}


/*
Arguments:
c: command processor structure
cmd: command to evaluate
pt: perhaps general game structure??
*/
int CoP_execute(CoP *c, char *cmd, void *pt) {
    char *verb=NULL;
    char *obj=NULL;
    char **str;
    int i=0;
    cmdfun_type f=NULL;
    ext_cmd *e;
    
    /*We separe verb and object*/
    verb = (char*)strdup(cmd);
    for(i=0;i<strlen(verb) && verb[i]!=' ';i++);
    if(verb[i]==' '){
        verb[i]=0;
        obj= (char*)strdup(strstr(cmd, " ")+1);
        obj[strlen(obj)-1]=0; /*Suprime el salto de línea*/
    }
    else{
        verb[i-1]=0;
        obj=(char*)strdup(" ");
    }

    /* First, search the list of external commands for the right one */
    e = _ext_src(c, verb);
    if (!e) {
        str = _unpack_all(c->error, verb);
        f = _assoc_search(c, c->error->internal);
        if(!f) {
            free(verb);
            free(obj);
            abort();
        }
        free(obj);
        return f(pt, verb, str, c->error->n_msg);
    }

    /* The command has been found */
    str = _unpack_all(e, obj);
    f = _assoc_search(c, e->internal);
    if(!f) {
        free(verb);
        free(obj);
        abort();
    }

    free(verb);
    return f(pt, obj, str, e->n_msg);
}


/*Converts * to words in a sentence with * 
Arguments:
-str: string with the * format sentence
-repl: word to which * will be converted
Return:
-A string with repl instead of *
*/
char *_unpack(char *str, char *repl) {
    char *ret, *p;
    int n = 0;

    /* count the number of stars in the first string */
    for (p=str; *p; p++) {
        if (*p == '*') 
            n++;
    }

    /* allocates the array with enough space for the final string (I add
    1 to store the final zero) */
    ret = (char *) malloc(strlen(str) + n*strlen(repl) + 1);
    p = ret;
    
    while (*str) {
        if (*str!='*') {
            *p++ = *str++;
        }
        else {
            strcpy(p, repl);
            p += strlen(repl);
            str++;
        }
    }

    *p=0;

    return ret;
}
