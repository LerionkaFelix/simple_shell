#include "shell.h"

/**
* allocateVal - allocate values to every member variable of the ShellVar struct
* @shell_var: pointer to ShellVar struct type that is to be assigned values
* @cmdline_args: command line arguments
* Return: void
*/

void allocateVal(ShellVar *shell_var, char **cmdline_args)
{
	unsigned int i;

	shell_var->cmd_count = 1;
	shell_var->exit_code = 0;
	shell_var->usr_input = NULL;
	shell_var->pid = sh_itoa(getpid());
	shell_var->cmdline_args = cmdline_args;
	shell_var->parsedArguments = NULL;

	for (i = 0; envt[i]; i++)
		;

	shell_var->environ_vars = malloc(sizeof(char *) * (i + 1));

	for (i = 0; envt[i]; i++)
		shell_var->environ_vars[i] = sh_strdup(envt[i]);

	shell_var->environ_vars[i] = NULL;
}
