#include "shell.h"

/**
 * get_next_cmd - traverses the CL_Node linked list and returns
 *                the next command to be executed
 * @head: beginning of the CL_Node linked list
 * @start: beginning of the SN_node linked list
 * @ptr: pointer to Shellvar struct
 * Return: void
 */
void get_next_cmd(CL_Node **head, SN_Node **hdr, ShellVar *ptr)
{
	CL_Node *current = *head;
	SN_Node *temp = *hdr;

	int sp_flag = 1;

	while (temp && sp_flag)
	{
		if ((*ptr).exit_code == 0)
		{
			if (temp->separatorChar == '&' || temp->separatorChar == ';')
				sp_flag = 0;
			if (temp->separatorChar == '|')
				current = current->next, temp = temp->next;
		}
		else
		{
			if (temp->separatorChar == '|' || temp->separatorChar == ';')
				sp_flag = 0;
			if (temp->separatorChar == '&')
				current = current->next, temp = temp->next;
		}
		if (temp && !sp_flag)
			temp = temp->next;
	}

	*head = current;
	*hdr = temp;
}

/**
 * input_encoder - encodes '|' and '&' separators as '0x10' and '0x0C' respectively
 *           for correct interpretation by the shell program
 * @inpstr: input string
 * @enflag: flag that determines whether to encode or decode
 * Return: encoded or decoded string
 */
char *input_encoder(char *inpstr, int enflag)
{
	int idx;

	if (enflag == 1)
	{
		idx = 0;
		while (inpstr[idx] != '\0')
		{
			if (inpstr[idx] == 16)
				inpstr[idx] = '|';
			else if (inpstr[idx] == 12)
				inpstr[idx] = '&';
			idx++;
		}
	}
	else
	{
		idx = 0;

		while (inpstr[idx] != '\0')
		{
			if (inpstr[idx] == '|')
			{
				if (inpstr[idx + 1] != '|')
					inpstr[idx] = 16;
				else
					idx++;
			}
			else if (inpstr[idx] == '&')
			{
				if (inpstr[idx + 1] != '&')
					inpstr[idx] = 12;
				else
					idx++;
			}
			idx++;
		}
	}
	return (inpstr);
}

/**
 * input_parser - parses input using the separators ';|&' and creates a node 
 *                for the parsed input
 * @head: head of the SN_Node list
 * @hdr: head of the CL_Node list
 * @input: user input
 * Return: void
 */
void input_parser(SN_Node **head, CL_Node **hdr, char *input)
{
	char dlmtrs[4] = ";|&";
	char *sprtkn;
	
	int idx = 0;

	input = input_encoder(input, 0);

	while (input[idx])
	{
		if (input[idx] == ';')
			add_SN_Node(head, input[idx]);

		if (input[idx] == '|' || input[idx] == '&')
		{
			add_SN_Node(head, input[idx]);
			idx++;
		}
		idx++;
	}

	sprtkn = str_tknzr(input, dlmtrs);

	while (sprtkn)
	{
		sprtkn = input_encoder(sprtkn, 1);
		add_CL_Node(hdr, sprtkn);
		sprtkn = str_tknzr(NULL, dlmtrs);
	}
}

/**
 * run_shell_cmd - splits multiple commands into separate commands using the 
 *          separator characters (;|&) for successful execution
 * @ptr: pointer to ShellVar struct
 * @input: command input
 * Return: 1 if execution was successfull, 0 otherwise
 */
int run_shell_cmd(ShellVar *ptr, char *input)
{
	CL_Node *h_node = NULL, *temp;
	SN_Node *head = NULL, *current;
	
	input_parser(&head, &h_node, input);
	temp = h_node;
	current = head;
	
	int flag = 0;

	while (temp)
	{
		(*ptr).usr_input = temp->command;
		(*ptr).parsedArguments = str_tokenize((*ptr).usr_input, TKN_DLMTR);

		flag = shell_execute(ptr);
		free((*ptr).parsedArguments);

		if (flag == 0)
			break;
		get_next_cmd(&temp, &current, ptr);
		if (temp)
			temp = temp->next;
	}

	free_SNode_Llist(&head);
	free_CL_Node_Llist(&h_node);
	if (flag == 0)
		return (0);
	return (1);
}
