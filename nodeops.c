#include "shell.h"

/**
 * add_CL_Node - adds commands node
 * @start: pointer to start of node
 * @command: command line string
 * Return: modified linked list, NULL otherwise
 */
CL_Node *add_CL_Node(CL_Node **start, char *cmd)
{
	CL_Node *new_node, *curr;

	if (!(*start))
	{
		*start = malloc(sizeof(CL_Node));
		if (!(*start))
			return (NULL);
		(*start)->command = cmd;
		(*start)->next = NULL;
		return (*start);
	}

	new_node = malloc(sizeof(CL_Node));

	if (!new_node)
		return (NULL);

	new_node->command = cmd;
	new_node->next = NULL;

	curr = *start;

	while (curr->next)
		curr = curr->next;

	curr->next = new_node;
	return (*start);
}

/**
 * add_SN_Node - adds SN_Node to a linked list
 * @head: pointer to beginning of list
 * @symbol: separator symbol
 * Return: modified SN_node linked list
 */
SN_Node *add_SN_Node(SN_Node **head, char sym)
{
	SN_Node *curr_node, *new_node = malloc(sizeof(SN_Node));

	if (!new_node)
		return (NULL);

	new_node->separatorChar = sym;
	new_node->next = NULL;

	curr_node = *head;
	if (!curr_node)
		*head = new_node;
	else
	{
		while (curr_node->next)
			curr_node = curr_node->next;

		curr_node->next = new_node;
	}

	return (*head);
}

/**
 * add_VarNode - adds a VarNode node to the linked list
 * @start: head of the list
 * @varlen: variable length
 * @vallen: length of the char value
 * @val: char value
 * Return: modified linked list or NULL otherwise
 */
VarNode *add_VarNode(VarNode **start, int varlen, char *val, int vallen)
{
	VarNode *temp, *new_node;

	new_node = malloc(sizeof(VarNode));

	if (!new_node)
		return (NULL);

	new_node->varlen = varlen;
	new_node->val = val;
	new_node->vallen = vallen;
	new_node->next = NULL;

	temp = *start;
	if (!temp)
		*start = new_node;
	else
	{
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}

	return (*start);
}

/**
 * free_CL_Node_Llist - Frees a CL_Node linked list
 * @start: pointer to the head of the CL_Node linked list
 * Return: void
 */
void free_CL_Node_Llist(CL_Node **start)
{
	CL_Node *curr;
	CL_Node *temp;
	
	if (start)
	{
		curr = *start;
		for (; curr; curr = temp)
		{
			temp = curr->next;
			free(curr);
		}
		*start = NULL;
	}
}

/**
 * free_ShellVar - Frees the memory allocated for the ShellVar struct
 * @ptr: A pointer to the ShellVar struct
 * Return: void
 */
void free_ShellVar(ShellVar *ptr)
{
	unsigned int idx = 0;

	while (ptr->environ_vars[idx])
	{
		free(ptr->environ_vars[idx]);
		idx++;
	}

	free(ptr->environ_vars);
	free(ptr->pid);
}

/**
 * free_SNode_Llist - Frees the allocated memory for the SN_Node
 *                    linked list
 * @head: Pointer to the head of the SN_Node linked list
 * Return: void
 */
void free_SNode_Llist(SN_Node **head)
{
	SN_Node *curr;
	SN_Node *temp;

	if (head)
	{
		curr = *head;
		for (; curr; curr = temp)
		{
			temp = curr->next;
			free(curr);
		}
		*head = NULL;
	}
}

/**
 * free_VarNode_Llist - frees a VarNode linked list
 * @start: pointer to the head of the VarNode linked list
 * Return: void
 */
void free_VarNode_Llist(VarNode **start)
{
	VarNode *curr, *prev;

	if (start)
	{
		curr = *start;

		do {
			prev = curr;
			curr = curr->next;
			free(prev);
		} while (curr);

		*start = NULL;
	}
}
