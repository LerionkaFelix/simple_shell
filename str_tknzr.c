#include "shell.h"

/**
 * str_tokenize - splits string input to an array of tokens
 * @input: pointer to input
 * @delim: delimiter string
 * Return: an array of strings
 */

char **str_tokenize(char *input, const char *delim)
{
	size_t buffsize = TKN_BUFFSIZE;
	size_t idx = 0;
	char **tokens = malloc(buffsize * sizeof(char *));
	char *tkn;

	if (!tokens)
	{
		dprintf(STDERR_FILENO, ": allocation error\n");
		exit(EXIT_FAILURE);
	}

	tkn = str_tknzr(input, delim);

	while (tkn)
	{
		tokens[idx] = tkn;
		idx++;

		if (idx >= buffsize)
		{
			buffsize += TKN_BUFFSIZE;
			tokens = str_reallocate(tokens, idx, buffsize * sizeof(char *));
			if (!tokens)
			{
				dprintf(STDERR_FILENO, ": allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		tkn = str_tknzr(NULL, delim);
	}

	tokens[idx] = NULL;

	return (tokens);
}

