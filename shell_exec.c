#include "shell.h"

/**
 * show_prompt - prints $ sign to initiate shell prompt
 * Return: void
 */
void show_prompt(void)
{
	dprintf(STDIN_FILENO, "~$ ");
}

/**
 * get_user_input - gets command line input from the user
 * @get_check: checks getline's return value
 * Return: read shell input
 */
char *get_user_input(int *get_check)
{
	size_t buff = 0;
	char *input = NULL;

	*get_check = getline(&input, &buff, stdin);

	return (input);
}

/**
 * comment_discard - discards comments present in the input string passed
 *                   to the function
 * @inp: input string
 * Return: comment with no comments
 * check if the sign is part of word if so it leaves
 */
char *comment_discard(char *inp)
{
	int comm_start = 0, idx = 0;

	while (inp[idx] != '\0')
	{
		if (inp[idx] == '#')
		{
			if (idx == 0)
			{
				free(inp);
				return (NULL);
			}
			if (inp[idx - 1] == ' ' || inp[idx - 1] == '\t' || inp[idx - 1] == ';')
			{
				comm_start = idx;
			}
		}
		idx++;
	}

	if (comm_start)
	{
		inp = mem_realloc(inp, idx, comm_start + 1);
		inp[comm_start] = '\0';
	}

	return (inp);
}


/**
 * shell_execution - handles the general shell program execution
 * @ptr: pointer to the ShellVar struct
 * Return: void
 */
void shell_execution(ShellVar *ptr)
{
	char *input;
	int getln_check, flag = 1;

	while (flag == 1)
	{
		show_prompt();

		input = get_user_input(&getln_check);
		if (getln_check != -1)
		{
			input = comment_discard(input);
			if (!input)
				continue;
			if (find_syntax_error(ptr, input) == 1)
			{
				(*ptr).exit_code = 2;
				free(input);
				continue;
			}
			input = var_xpan_replace(input, ptr);
			flag = run_shell_cmd(ptr, input);
			(*ptr).cmd_count += 1;
			free(input);
		}
		else
		{
			flag = 0;
			free(input);
		}
	}
}
