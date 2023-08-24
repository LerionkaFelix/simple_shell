#include "shell.h"

/**
 * builtin_check - compare the command string passed to it as an argument
 *                 with an array of BuiltInCmd structures containing the 
 *                 names of the built-in commands and their corresponding 
 *                 function pointers
 * @name: command string
 * Return: The corresponding function pointer if the command string is found,
 *         otherwise NULL is returned
 */
int (*builtin_check(char *name))(ShellVar *)
{
	int i;

	BuiltInCmd builtIns[] = {
		{ "cd", change_argsdir },
		{ "help", print_shellcmd_info },
		{ "exit", sh_exit_handler },
		{ "env", sh_env },
		{ "setenv", set_envname },
		{ "unsetenv", unset_envvar },
		{ NULL, NULL }
	};

	for (i = 0; builtIns[i].cmdname != NULL; i++)
	{
		if (sh_strcmp(builtIns[i].cmdname, name) == 0)
			return (builtIns[i].func);
	}

	return (NULL);
}

/**
 * sh_env - prints the shell environment variables
 * @ptr: pointer to the ShellVar struct
 * Return: 1
 */

int sh_env(ShellVar *ptr)
{
	int x = 0, y = 0;

	while ((*ptr).environ_vars[x])
	{
		y = 0;
		
		while ((*ptr).environ_vars[x][y])
			y++;

		dprintf(STDOUT_FILENO, "%s\n", (*ptr).environ_vars[x]);
		x++;
	}

	(*ptr).exit_code = 0;
	return (1);
}


/**
 * sh_exit_handler - Handles the exit builtin command
 * @ptr: A pointer to a data_shell struct
 * Return: Always returns 0 to signal the shell to exit
 */
int sh_exit_handler(ShellVar *ptr)
{
	unsigned int exit_status;
	int isdigit, is_largenum, str_length;

	if ((*ptr).parsedArguments[1] != NULL)
	{
		exit_status = sh_atoi((*ptr).parsedArguments[1]);
		isdigit = sh_isdigit((*ptr).parsedArguments[1]);
		is_largenum = exit_status > (unsigned int)INT_MAX;
		str_length = sh_strlen((*ptr).parsedArguments[1]);
		
		if (!isdigit || str_length > 10 || is_largenum)
		{
			error_handler(ptr, 2);
			(*ptr).exit_code = 2;
			return (1);
		}

		(*ptr).exit_code = (exit_status % 256);
	}

	return (0);
}
