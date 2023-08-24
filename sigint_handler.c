#include "shell.h"

/**
 * intsignal_handler - handling CTRL C signal
 * @sigint: resulting signal number produced when pressing CTRL C
 * Return: void
 */

void intsignal_handler(__attribute__((unused))int sigint)
{
	dprintf(STDOUT_FILENO, "\n~$ ");
}
