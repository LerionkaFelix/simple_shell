#include "shell.h"

/**
 * get_sh_cmdname - checks the provided string argument if it 
 *                  matches a command in the shell program
 * @arg: string argument
 * Return: the shell command value if found, otherwise -1
 */
SHCommandType get_sh_cmdname(char *arg)
{
	if (sh_strcmp(arg, "setenv") == 0)
	{
		return (SH_CMD_SETENV);
	}
	else if (sh_strcmp(arg, "env") == 0)
	{
		return (SH_CMD_ENV);
	}
	else if (sh_strcmp(arg, "unsetenv") == 0)
	{
		return (SH_CMD_UNSETENV);
	}
	else if (sh_strcmp(arg, "help") == 0)
	{
		return (SH_CMD_HELP);
	}
	else if (sh_strcmp(arg, "exit") == 0)
	{
		return (SH_CMD_EXIT);
	}
	else if (sh_strcmp(arg, "cd") == 0)
	{
		return (SH_CMD_CD);
	}
	else if (sh_strcmp(arg, "alias") == 0)
	{
		return (SH_CMD_ALIAS);
	}
	else
	{
		return (-1);
	}
}

/**
 * print_setenv_info - prints the "setenv" shell command help info
 * Return: void
 */
void print_setenv_info(void)
{
	dprintf(STDOUT_FILENO, "setenv: setenv (const char *name, const char *value, int replace)");
	dprintf(STDOUT_FILENO, "\n\tAdd a new definition to the environment\n");
}

/**
 * print_env_info - prints the "env" shell command help info
 * Return: void
 */
void print_env_info(void)
{
	dprintf(STDOUT_FILENO, "env: env [option] [name=value] [command [args]]");
	dprintf(STDOUT_FILENO, "\n\tPrint the environment of the shell.\n");
}

/**
 * print_unsetenv_info - prints the "unsetenv" shell command help info
 * Return: void
 */
void print_unsetenv_info(void)
{
	dprintf(STDOUT_FILENO, "unsetenv: unsetenv (const char *name)");
	dprintf(STDOUT_FILENO, "\n\tRemove an input entirely from the environment\n");
}

/**
 * print_help_info - prints the "help" shell command info
 * Return: void
 */
void print_help_info(void)
{
	dprintf(STDOUT_FILENO, "help: help [-dms] [pattern ...]\n");
	dprintf(STDOUT_FILENO, "\tDisplays brief info on the builtin commands.\n");
}

/**
 * print_help_exit - prints the "exit" shell command description
 * Return: void
 */
void print_help_exit(void)
{
	dprintf(STDOUT_FILENO, "exit: exit [n]\n\tExit shell.");
	dprintf(STDOUT_FILENO, "\n\tExits the shell with a status of n.");
	dprintf(STDOUT_FILENO, "\n\tIf n is omitted, the last command's exit status is used.\n");
}

/**
 * print_help_cd - prints the "cd" shell command description
 * Return: void
 */
void print_help_cd(void)
{
	dprintf(STDOUT_FILENO, "cd: cd [-L|[-P [-e]] [-@]] [dir]");
	dprintf(STDOUT_FILENO, "\n\tChange the shell working directory.\n");
}

/**
 * print_help_alias - prints the "alias" shell command description
 * Return: void
 */
void print_help_alias(void)
{
	dprintf(STDOUT_FILENO, "alias: alias [-p] [name[=value]...]");
	dprintf(STDOUT_FILENO, "\n\tDefine or display aliases.\n");
}

/**
 * bash_descriptor - ptints the general bash commands description
 * Return: void
 */
void bash_descriptor(void)
{
	dprintf(STDOUT_FILENO, "~$ bash, version 1.0(1)-release");
	dprintf(STDOUT_FILENO, "\nThese commands are defined internally.");
	dprintf(STDOUT_FILENO, "Type 'help' to display list pof commands.\n");
	dprintf(STDOUT_FILENO, "Type 'help cmd' to find out more about the command 'cmd'.\n\n");
	dprintf(STDOUT_FILENO, "alias: alias [name=['string']]\n");
	dprintf(STDOUT_FILENO, "cd: cd [-L|[-P [-e]] [-@]] [dir]\n");
	dprintf(STDOUT_FILENO, "exit: exit [n]\n");
	dprintf(STDOUT_FILENO, "env: env [opt] [name=value] [command [args]]\n");
	dprintf(STDOUT_FILENO, "setenv: setenv [var] [value]\n");
	dprintf(STDOUT_FILENO, "unsetenv: unsetenv [var]\n");
}

/**
 * print_shellcmd_info - print the help info for a specific shell command or 
 *                       general information on how to use the shell program.
 * @ptr: pointer to a ShellVar
 * Return: 1 once completed
 */
int print_shellcmd_info(ShellVar *ptr)
{
	char *arg = (*ptr).parsedArguments[1];
	int status = 0;

	if (!arg)
		bash_descriptor();
	else
	{
		switch (get_sh_cmdname(arg))
		{
			case SH_CMD_SETENV:
				print_setenv_info();
				break;
			case SH_CMD_ENV:
				print_setenv_info();
				break;
			case SH_CMD_UNSETENV:
				print_unsetenv_info();
				break;
			case SH_CMD_HELP:
				print_help_info();
				break;
			case SH_CMD_EXIT:
				print_help_exit();
				break;
			case SH_CMD_CD:
				print_help_cd();
				break;
			case SH_CMD_ALIAS:
				print_help_alias();
				break;
			default:
				dprintf(STDERR_FILENO, "%s", (*ptr).parsedArguments[0]);
				status = 1;
				break;
		}
	}
	(*ptr).exit_code = status;
	return (1);
}
