#include "shell.h"

/**
 * main - responsible for program execution
 * @argv: list of command line arguments
 * @argc: number of command line arguments
 * Return: 0
 */

int main(__attribute__((unused))int argc, char *argv[])
{
	ShellVar shell_data;

	signal(SIGINT, intsignal_handler);
	allocateVal(&shell_data, argv);
	shell_execution(&shell_data);
	free_ShellVar(&shell_data);
	if (shell_data.exit_code < 0)
		return (255);
	return (shell_data.exit_code);
}
