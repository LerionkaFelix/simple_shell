#ifndef SHELL_H
#define SHELL_H

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

#define TKN_BUFFSIZE 128
#define BUFF_SIZE 1024
#define TKN_DLMTR " \t\r\n\a"

extern char **envt;

void intsignal_handler(int sigint);

/**
 * struct ShellVar - a group of arguments and variables used within a shell program
 * @cmd_count: number of commands to be excecuted
 * @exit_code: the last command's exit status
 * @usr_input: a pointer to the command line input
 * @pid: a pointer to the shell program's PID
 * @cmdline_args: a pointer to command line arguments
 * @parsedArguments: a pointer to arguments parsed from the user input
 * @environ_vars: a pointer to the environment variables passed to the program
 */
typedef struct ShellVar
{
	int cmd_count;
	int exit_code;
	char *usr_input;
	char *pid;
	char **cmdline_args;
	char **parsedArguments;
	char **environ_vars;
} ShellVar;


/**
 * struct BuiltInCmd - info for built-in commands
 * @name: name of the built-in command
 * @func: pointer to the corresponding built-in function
 */
typedef struct BuiltInCmd
{
	char *cmdname;
	int (*func)(ShellVar *ptr);
} BuiltInCmd;

/**
 * struct VarNode - a node in a singly linked list that stores variables
 * @varlen: variable length
 * @vallen: length of the variable's value 
 * @val: value of the variable
 * @next: pointer to the next node in the linked list
 */
typedef struct VarNode
{
	int varlen;
	int vallen;
	char *val;
	struct VarNode *next;
} VarNode;

/**
 * struct SN_Node - node for a singly linked list that stores separators,
 * responsible for separating commands into different parts for
 * separate execution
 * @symbol: | & ;
 * @next: pointer to next node
 */
typedef struct SN_Node
{
	char separatorChar;
	struct SN_Node *next;
} SN_Node;

/**
 * struct CL_Node - node within a singly linked list that stores command line
 *                  strings and a pointer to the next command in the list
 * @command: the command line string
 * @next: pointer to the next node in the list
 */
typedef struct CL_Node
{
	char *command;
	struct CL_Node *next;
} CL_Node;

/**
 * enum SHCommandType - Represents the type of a shell command.
 * @SH_CMD_SETENV: The "setenv" command.
 * @SH_CMD_ENV: The "env" command.
 * @SH_CMD_UNSETENV: The "unsetenv" command.
 * @SH_CMD_HELP: The "help" command.
 * @SH_CMD_EXIT: The "exit" command.
 * @SH_CMD_CD: The "cd" command.
 * @SH_CMD_ALIAS: The "alias" command.
 * Description: This enumeration represents the different types of commands available
 *              in the shell program. Each enumerated value corresponds to a specific
 *              command.
 */
typedef enum SHCommandType
{
	SH_CMD_CD,
	SH_CMD_HELP,
	SH_CMD_ALIAS,
	SH_CMD_EXIT,
	SH_CMD_ENV,
	SH_CMD_SETENV,
	SH_CMD_UNSETENV
} SHCommandType;

/*allocate.c*/
void allocateVal(ShellVar *shell_var, char **cmdline_args);

/*builtin.c*/
int sh_exit_handler(ShellVar *ptr);
int (*builtin_check(char *name))(ShellVar *);
int sh_env(ShellVar *ptr);

/*cmd_exec.c*/
void get_next_cmd(CL_Node **head, SN_Node **hdr, ShellVar *ptr);
char *input_encoder(char *input, int encode);
void input_parser(SN_Node **head, CL_Node **hdr, char *input);
int run_shell_cmd(ShellVar *ptr, char *input);

/*dir_fs_ops.c*/
void change_destdir(ShellVar *ptr);
void change_homedir(ShellVar *ptr);
void change_pardir(ShellVar *ptr);
void change_upperdir(ShellVar *ptr);
int change_argsdir(ShellVar *datash);

/*env_ops.c*/
void check_envvar(VarNode **start, char *instr, ShellVar *ptr);
char *envvar_xpan(VarNode **start, const char *instr, char *newstr, int new_len);
int sym_parser(VarNode **head, char *input, char *status, ShellVar *ptr);
char *var_xpan_replace(char *input, ShellVar *ptr);

/*generr.c*/
char *generr_cd(ShellVar *ptr);
char *generr_env(ShellVar *ptr);
char *generr_permissions(ShellVar *ptr);
char *generr_cmd_notfound(ShellVar *ptr);
char *generr_exit_shell(ShellVar *ptr);
int cmd_search_err(char *dir, ShellVar *ptr);

/*handle_error.c*/
int error_handler(ShellVar *prt, int err_num);

/*memalloc.c*/
void *mem_realloc(void *optr, size_t osize, size_t nsize);
char **dptr_realloc_sh(char **dptr, unsigned int dptr_size, unsigned int new_size);
char **str_reallocate(char **strarr_ptr, unsigned int osize, unsigned int nsize);

/*nodeops.c*/
CL_Node *add_CL_Node(CL_Node **start, char *cmd);
SN_Node *add_SN_Node(SN_Node **head, char sym);
VarNode *add_VarNode(VarNode **start, int varlen, char *val, int vallen);
void free_CL_Node_Llist(CL_Node **start);
void free_ShellVar(ShellVar *ptr);
void free_SNode_Llist(SN_Node **head);
void free_VarNode_Llist(VarNode **start);

/*runshell.c*/
int exec_shell_cmd(ShellVar *shell_var);
char *get_cmd_loc(char *cmd, char **envp);
int is_executable_cmd(char *command, ShellVar *ptr);
int path_seg_check(char *path, int *index);
int shell_execute(ShellVar *ptr);

/*setup_env.c*/
char *create_new_envvar(char *key, char *val);
char *get_envname(const char *env_name, char **env_arr);
int set_envname(ShellVar *ptr);
void set_envvar(char *key, char *value, ShellVar *ptr);
int unset_envvar(ShellVar *ptr);

/*shell_exec.c*/
void show_prompt(void);
char *get_user_input(int *get_check);
char *comment_discard(char *inp);
void shell_execution(ShellVar *ptr);

/*shell_help.c*/
SHCommandType get_sh_cmdname(char *arg);
void print_setenv_info(void);
void print_env_info(void);
void print_unsetenv_info(void);
int print_shellcmd_info(ShellVar *ptr);
void print_help_info(void);
void print_help_exit(void);
void print_help_cd(void);
void print_help_alias(void);
void bash_descriptor(void);

/*sh_strop.c*/
int sh_strlen(const char *str);
char *sh_strdup(char *str);
char *sh_strncpy(char *dest, char *src, int n);
char *sh_strchr(char *str, char c);
char *str_tknzr(char *str, const char *dlmtr);
char *sh_strndup(char *str, int i);
char *sh_itoa(int num);
void sh_strrev(char *str);
unsigned int sh_strspn(char *str, const char *match);
int sh_isdigit(const char *str);
int get_num_len(int num);
int sh_atoi(const char *str);
char *sh_strbytematch(char *str, const char *charset);
int sh_strcmp(const char *str1, const char *str2);
char *sh_strcpy(char *dest, const char *src);
char *sh_memcpy(char *dest, char *src, unsigned int n);

/*str_tokenize.c*/
char **str_tokenize(char *input, const char *delim);

/*syntax_error.c*/
int duplicate_check(char *input, int i);
int separator_check(char *iunput, int idx, char offset);
int first_char_check(char *cmd_input, int *offset);
int find_syntax_error(ShellVar *ptr_data, char *input);
void generate_errmsg(ShellVar *ptr, char *input, int index, int rs_flag);


#endif /*SHELL_H*/
