#ifndef COP_H
#define COP_H

typedef struct _Ext_cmd ext_cmd;
typedef struct _Int_cmd int_cmd;
typedef struct _CoP CoP;


typedef int (*cmdfun_type) (void *, char *, char **, int );



void CoP_free(CoP * command);
CoP *CoP_create(FILE *cmdfile);
int CoP_assoc(CoP *c, char *int_name, cmdfun_type cfun);
int CoP_execute(CoP *c, char *cmd, void *pt);
char *_unpack(char *str, char *repl);


#endif

