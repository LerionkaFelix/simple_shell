#include "shell.h"

/**
 * generr_cd - generate error message related to the cd command
 * @ptr: pointer to ShellVar struct
 * Return: error message
 */
char *generr_cd(ShellVar *ptr)
{
	char *emsg, *parsedArg, *parsedArg1, *err, *cargs, *s;
	int length, len;

	s = sh_itoa(ptr->cmd_count);

	if (ptr->parsedArguments[1][0] == '-')
	{
		emsg = ": Illegal option ";
		length = 2;
	}
	else
	{
		emsg = ": can't change directory to ";
		length = sh_strlen(ptr->parsedArguments[1]);
	}

	cargs = ptr->cmdline_args[0];
	parsedArg = ptr->parsedArguments[0];
	len = sh_strlen(cargs) + sh_strlen(parsedArg);
	len += sh_strlen(s) + sh_strlen(emsg) + length + 5;
	err = malloc(sizeof(char) * (len + 1));
	
	if (!err)
	{
		free(s);
		return (NULL);
	}

	parsedArg1 = ptr->parsedArguments[1];
	sprintf(err, "%s: %s: %s%s%s", cargs, s, emsg, parsedArg1, "\n");

	free(s);
	return (err);
}

/**
 * generr_env - generates an environ-related error message
 * @ptr: pointer to ShellVar struct
 * Return: error message
 */
char *generr_env(ShellVar *ptr)
{
	char *msg, *cargs, *parsedArg, *err, *s;
	int len;

	s = sh_itoa(ptr->cmd_count);
	msg = ": Unable to add/remove from environment\n";
	len = sh_strlen(ptr->cmdline_args[0]) + sh_strlen(s);
	len += sh_strlen(ptr->parsedArguments[0]) + sh_strlen(msg) + 2;
	err = malloc(sizeof(char) * (len + 1));

	if (!err)
	{
		free(err);
		free(s);
		return (NULL);
	}
	
	cargs = ptr->cmdline_args[0];
	parsedArg = ptr->parsedArguments[0];
	sprintf(err, "%s: %s: %s%s", cargs, s, parsedArg, msg);
	
	free(s);
	return (err);
}

/**
 * generr_permissions - generates an error message when a command cannot be executed
 *                      due to inadequate permissions
 * @ptr: pointer to ShellVar struct
 * Return: the error message or NULL otherwise
 */
char *generr_permissions(ShellVar *ptr)
{
	char *p_str, *err;
	int len;

	p_str = sh_itoa(ptr->cmd_count);
	len = sh_strlen(ptr->cmdline_args[0]) + sh_strlen(p_str) + 27;
	err = malloc(sizeof(char) * (len + 1));
	
	if (!err)
	{
		free(err);
		free(p_str);
		return (NULL);
	}
	
	sprintf(err, "%s: %s: %s: Permission denied\n",
	    ptr->cmdline_args[0], p_str, ptr->cmdline_args[0]);
	
	free(p_str);
	return (err);
}

/**
 * generr_cmd_notfound - generate error message if command isn't found
 * @ptr: pointer to ShellVar struct
 * Return: the error message or NULL otherwise
 */
char *generr_cmd_notfound(ShellVar *ptr)
{
	char *err, *e_str;
	int len;

	char *cargs = ptr->cmdline_args[0];
	char *parsedArg = ptr->parsedArguments[0];

	e_str = sh_itoa(ptr->cmd_count);
	len =  sh_strlen(cargs) + sh_strlen(e_str) + sh_strlen(parsedArg) + 1;
	err = malloc(sizeof(char) * (len + 14));
	
	if (!err)
	{
		free(e_str);
		return (NULL);
	}

	sprintf(err, "%s: %s: %s: not found\n", cargs, e_str, parsedArg);
	free(e_str);
	return (err);
}

/**
 * generr_exit_shell - generate error message related to exiting 
 *                     shell message
 * @ptr: pointer to ShellVar struct
 * Return: the error message or NULL otherwise
 */
char *generr_exit_shell(ShellVar *ptr)
{
	char *err, *errstr;
	char *cargs = ptr->cmdline_args[0];
	char *parsedArg = ptr->parsedArguments[0];
	char *parsedArg1 = ptr->parsedArguments[1];
	int len;
	
	errstr = sh_itoa(ptr->cmd_count);
	len = sh_strlen(cargs) + sh_strlen(errstr) + sh_strlen(parsedArg);
	len += sh_strlen(parsedArg1) + 1;
	err = malloc(sizeof(char) * (len + 25));
	
	if (!err)
	{
		free(errstr);
		return (NULL);
	}

	snprintf(err, len + 25, "%s: %s: %s: Illegal number: %s\n",
		 cargs, errstr, parsedArg, parsedArg1);

	free(errstr);
	return (err);
}

/**
 * cmd_search_err - checks if a command directory exists
 * @dir: directory of the command
 * @ptr: pointer to ShellVar struct
 * Return: 1 in case of an error, otherwise 0
 */
int cmd_search_err(char *dir, ShellVar *ptr)
{
	if (!dir)
	{
		error_handler(ptr, 127);
		return (1);
	}

	if (sh_strcmp((*ptr).parsedArguments[0], dir) != 0)
	{
		if (access(dir, X_OK) == -1)
		{
			error_handler(ptr, 126);
			free(dir);
			return (1);
		}
		free(dir);
	}
	else
	{
		if (access((*ptr).parsedArguments[0], X_OK) == -1)
		{
			error_handler(ptr, 126);
			return (1);
		}
	}

	return (0);
}
