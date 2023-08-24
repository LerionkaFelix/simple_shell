#include "shell.h"

/**
 * duplicate_check - count the number of the same consecutive characters
 *              in the input string
 * @input: cmd input
 * @i: index
 * Return: number of occurences
 */
int duplicate_check(char *input, int i)
{
	while (*(input - 1) == *input)
	{
		i++;
		input--;
	}
	return (i);
}

/**
 * separator_check - checks for separator-related syntax errors
 * @input: pointer to user input
 * @idx: index
 * @offset: tracks the last separator checked in the command
 * Return: index where the error was found, otherwise 0
 */
int separator_check(char *uinput, int idx, char offset)
{
	int count;

	while (*uinput != '\0')
	{
		if (*uinput == ' ' || *uinput == '\t')
		{
			uinput++, idx++;
			continue;
		}
		if (*uinput == ';')
		{
			if (offset == '|' || offset == '&' || offset == ';')
				return (idx);
		}
		else if (*uinput == '|')
		{
			if (offset == ';' || offset == '&')
				return (idx);
			if (offset == '|')
			{
				count = duplicate_check(uinput, 0);
				if (count == 0 || count > 1)
					return (idx);
			}
		}
		else if (*uinput == '&')
		{
			if (offset == ';' || offset == '|')
				return (idx);
			if (offset == '&')
			{
				count = duplicate_check(uinput, 0);
				if (count == 0 || count > 1)
					return (idx);
			}
		}
		offset = *uinput;
		uinput++, idx++;
	}
	return (0);
}

/**
 * first_char_check - check if first character of the each cmd input
 *                    is a separator (| & ;)
 * @cmd_input: pointer to the command input string
 * @offset: pointer to the index of the first separator character in 
 *          the command input string
 * Return: -1 if there's a separator, otherwise 0 
 */
int first_char_check(char *cmd_input, int *offset)
{
	char *temp = cmd_input;

	while (*temp != '\0')
	{
		if (*temp == ' ' || *temp == '\t')
		{
			temp++;
			continue;
		}

		if (*temp == '|' || *temp == '&' || *temp == ';')
		{
			*offset = temp - cmd_input;
			return (-1);
		}

		*offset = temp - cmd_input;
		return (0);
	}
	return (0);
}

/**
 * generate_errmsg - print an error msg with info about the syntax error
 * @ptr: pointer to the ShellVar struct
 * @input: pointer to the input command string
 * @index: represents the index of the character in the command string
 *         that causes the syntax error
 * @rs_flag: flag used to determine the position of the repeated sequence of symbols
 *         resulting in the syntax error
 * Return: void
 */

void generate_errmsg(ShellVar *ptr, char *input, int index, int rs_flag)
{
	char *err, *cmd_counter, *sh;
	char *msg1, *msg2, *msg3;
	int len;

	if (input[index] == ';')
	{
		if (rs_flag == 0)
			msg1 = (input[index + 1] == ';' ? ";;" : ";");
		else
			msg1 = (input[index - 1] == ';' ? ";;" : ";");
	}

	if (input[index] == '|')
		msg1 = (input[index + 1] == '|' ? "||" : "|");

	if (input[index] == '&')
		msg1 = (input[index + 1] == '&' ? "&&" : "&");

	msg2 = ": syntax error: \"";
	msg3 = "\" unexpected\n";
	cmd_counter = sh_itoa(ptr->cmd_count);
	len = sh_strlen(ptr->cmdline_args[0]) + sh_strlen(cmd_counter);
	len += sh_strlen(msg1) + sh_strlen(msg2) + sh_strlen(msg3) + 2;

	err = malloc(sizeof(char) * (len + 1));
	if (err == 0)
	{
		free(cmd_counter);
		return;
	}

	sh = ptr->cmdline_args[0];
	sprintf(err, "%s: %s%s%s%s", sh, cmd_counter, msg2, msg1, msg3);
	err[len] = '\0';
	dprintf(STDERR_FILENO, "%s ", err);
	free(err);
	free(cmd_counter);
}

/**
 * find_syntax_error - checks for syntax error with the help of helper funcs
 * @ptr: pointer to a ShellVar struct
 * @input: command input
 * Return: 1 in case of an error, 0 otherwise
 */
int find_syntax_error(ShellVar *ptr, char *cmd_input)
{
	int offset = 0;
	int res = first_char_check(cmd_input, &offset);

	if (res == -1)
	{
		generate_errmsg(ptr, cmd_input, offset, 0);
		return (1);
	}

	res = separator_check(cmd_input + offset, 0, *(cmd_input + offset));

	if (res != 0)
	{
		generate_errmsg(ptr, cmd_input, offset, 1);
		return (1);
	}
	return (0);
}
