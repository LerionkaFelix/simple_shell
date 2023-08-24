#include "shell.h"

/**
 * check_envvar - checks if the user input corresponds to an existing 
 *                  environment variable name.
 * @start: double pointer to the beginning of the VarNode linked list
 * @instr: a pointer to the input string
 * @ptr: a pointer to a ShellVar struct
 */
void check_envvar(VarNode **start, char *instr, ShellVar *ptr)
{
	char **env_vars = (*ptr).environ_vars;
	int vallen, idx, idx1;

	for (idx = 0; env_vars[idx]; idx++)
	{
		idx1 = 0;
		while (env_vars[idx][idx1] && instr[idx1] && env_vars[idx][idx1] == instr[idx1])
			idx1++;

		if (env_vars[idx][idx1] == '=')
		{
			vallen = sh_strlen(env_vars[idx] + idx1 + 1);
			add_VarNode(start, idx1, env_vars[idx] + idx1 + 1, vallen);
			return;
		}
	}

	idx1 = 0;
	while (instr[idx1] != '\0')
	{
		if (instr[idx1] != ' ' || instr[idx1] != '\t' || instr[idx1] != ';' || instr[idx1] != '\n')
			break;
		idx1++;
	}
	add_VarNode(start, idx1, NULL, 0);
}

/**
 * envvar_xpan - expands environment variables in the input string
 * @start: pointer to the head of a VarNode linked list
 * @instr: pointer to the input string
 * @newstr: pointer to the memory location that'll store expanded string
 * @new_len: length of newstr
 * Return: new string
 y*/
char *envvar_xpan(VarNode **start, const char *instr, char *newstr, int new_len)
{
	VarNode *hdvar = *start;
	int idx = 0, idx1 = 0;
	int idx2;

	while (instr[idx] != '\0' && idx1 < new_len - 1)
	{
		if (instr[idx] == '$')
		{
			if (!(hdvar->varlen) && !(hdvar->vallen))
			{
				newstr[idx1] = instr[idx];
				idx++;
				idx1++;
			}
			else if (hdvar->varlen && !(hdvar->vallen))
			{
				for (idx2 = 0; idx2 < hdvar->varlen && idx1 < new_len - 1; idx2++, idx1++)
					newstr[idx1] = instr[idx + idx2];
				idx += hdvar->varlen;
			}
			else
			{
				for (idx2 = 0; idx2 < hdvar->vallen && idx1 < new_len - 1; idx2++, idx1++)
					newstr[idx1] = hdvar->val[idx2];
				idx += hdvar->varlen;
			}
			hdvar = hdvar->next;
		}
		else
		{
			newstr[idx1] = instr[idx];
			idx++;
			idx1++;
		}
	}

	newstr[idx1] = '\0';
	return (newstr);
}

/**
 * sym_parser - parses special characters in the input string, and
 *              adds a new node in a VarNode linked list
 * @head: pointer to the head of a VarNode linked list
 * @input: user input
 * @status: current Shell status
 * @ptr: pointer to ShellVar struct 
 * Return: current index in the input string
 */
int sym_parser(VarNode **head, char *input, char *status, ShellVar *ptr)
{
	int len_pid = 0, len_stat = 0;
	int idx;

	len_pid = sh_strlen(ptr->pid);
	len_stat = sh_strlen(status);
	
	for (idx = 0; input[idx]; idx++)
	{
		if (input[idx] == '$')
		{
			if (input[idx + 1] == '?')
				add_VarNode(head, 2, status, len_stat);
			else if (input[idx + 1] == '$')
				add_VarNode(head, 2, ptr->pid, len_pid);
			else if (input[idx + 1] == '\n' || input[idx + 1] == '\0')
				add_VarNode(head, 0, NULL, 0);
			else if (input[idx + 1] == ' ' || input[idx + 1] == '\t' || input[idx + 1] == ';')
				add_VarNode(head, 0, NULL, 0);
			else
				check_envvar(head, input + idx, ptr);
		}
	}
	return (idx);
}

/**
 * var_xpan_replace - expands input string and replaces the shell variables with
 *                    the existing values in the environment variables array
 * @input: pointer to the input string
 * @ptr: pointer to ShellVar struct
 * Return: a new string with the substituted shell values
 */
char *var_xpan_replace(char *input, ShellVar *ptr)
{
	VarNode *head, *temp;
	char *newstr, *exit_state;
	int idx1, idx2;

	exit_state = sh_itoa((*ptr).exit_code);
	head = NULL;
	idx1 = sym_parser(&head, input, exit_state, ptr);

	if (!head)
	{
		free(exit_state);
		return (input);
	}
	temp = head;
	idx2 = 0;

	while (temp != NULL)
	{
		idx2 += (temp->vallen - temp->varlen);
		temp = temp->next;
	}
	idx2 += idx1;
	newstr = malloc(sizeof(char) * (idx2 + 1));

	newstr = envvar_xpan(&head, input, newstr, idx2);

	free(input);
	free(exit_state);
	free_VarNode_Llist(&head);
	return (newstr);
}
