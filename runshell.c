#include "shell.h"

/**
 * exec_shell_cmd - handles shell command execution; checks if the cmd exists
 *                  and sets its path accordingly. The function forks a child
 *                  process and executes the failed command with the execve 
 *                  system call
 * @shell_data: pointer to ShellVar struct
 * Return: 1 if the command is executable or not
 */
int exec_shell_cmd(ShellVar *shell_var)
{
	char *dir;
	int status, ex_flag;
	pid_t pid, pid_wp;
	
	char *inputArg = shell_var->parsedArguments[0];
	char **env_arr = shell_var->environ_vars;

	(void) pid_wp;
	ex_flag = is_executable_cmd(inputArg, shell_var);
	if (ex_flag == -1)
		return (1);

	if (ex_flag == 0)
	{
		dir = get_cmd_loc(inputArg, env_arr);
		if (cmd_search_err(dir, shell_var) == 1)
			return (1);
	}

	pid = fork();
	if (pid == 0)
	{
		if (ex_flag == 0)
			dir = get_cmd_loc(inputArg, env_arr);
		else
			dir = inputArg;
		execve(dir + ex_flag, shell_var->parsedArguments, env_arr);
	}
	else if (pid < 0)
	{
		perror(shell_var->cmdline_args[0]);
		return (1);
	}
	else
	{
		do {
			pid_wp = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	shell_var->exit_code = (status / 256);
	return (1);
}

/**
 * get_cmd_loc - used to search for a particular command's location in
 *               the PATH environ variable
 * @cmd: command to be searched
 * @env_arr: pointer to environment variables array
 * Return: path to the executable command's location, NULL otherwise
 */
char *get_cmd_loc(char *cmd, char **env_arr)
{
	char *dir, *path, *path_ptr, *tkn;
	int len_cmd, len_dir, idx;

	struct stat cmd_stat;

	path = get_envname("PATH", env_arr);
	if (path)
	{
		path_ptr = sh_strdup(path);
		len_cmd = sh_strlen(cmd);
		tkn = str_tknzr(path_ptr, ":");
		
		idx = 0;
		do {
			if (path_seg_check(path, &idx))
			{
				if (stat(cmd, &cmd_stat) == 0)
					return (cmd);
			}

			len_dir = sh_strlen(tkn);
			dir = malloc(len_dir + len_cmd + 2);
			sprintf(dir, "%s/%s", tkn, cmd);

			if (stat(dir, &cmd_stat) == 0)
			{
				free(path_ptr);
				return (dir);
			}

			free(dir);
			tkn = str_tknzr(NULL, ":");
		} while (tkn);

		free(path_ptr);
		if (stat(cmd, &cmd_stat) == 0)
			return (cmd);

		return (NULL);
	}
	if (cmd[0] == '/')
	{
		if (stat(cmd, &cmd_stat) == 0)
			return (cmd);
	}
	return (NULL);
}

/**
 * is_executable_cmd - checks if a command is executable
 * @command: parsed input as a result of separator operators
 * @ptr: pointer to ShellVar struct
 * Return: index of the filename in the parsed input, -1 otherwise
 */
int is_executable_cmd(char *cmd, ShellVar *ptr)
{
	struct stat cmd_stat;
	int idx, temp_idx = -1;

	for (idx = 0; cmd[idx]; idx++)
	{
		if (cmd[idx] == '.')
		{
			if (cmd[idx + 1] == '.')
				return (-1);
			if (cmd[idx + 1] == '/')
				continue;
			else
				break;
		}
		else if (cmd[idx] == '/' && idx != 0)
		{
			if (cmd[idx + 1] == '.')
				continue;
			idx++;
			temp_idx = idx;
			break;
		}
		else
			break;
	}
	if (temp_idx == -1)
		return (0);

	if (stat(cmd + temp_idx, &cmd_stat) == 0)
	{
		if (S_ISREG(cmd_stat.st_mode) && (cmd_stat.st_mode & S_IXUSR))
			return (temp_idx);
	}
	error_handler(ptr, 127);
	return (-1);
}

/**
 * path_seg_check - checks if the provided path segment is enclosed within
 *                   colons ':' 
 * @path: path segment string
 * @index: pointer to current index in the provided path segment
 * Return: 1 if the path segment starts or ends with ':', 0 otherwise
 */
int path_seg_check(char *path_seg, int *idx)
{
	if (path_seg[*idx] == ':')
		return (1);

	for (; path_seg[*idx]; (*idx)++)
	{
		if (path_seg[*idx] == ':')
			return (1);
	}

	return (0);
}

/**
 * shell_execute - shell_execute commands based on the provided arguments
 * @ptr: pointer to ShellVar struct
 * Return: 1
 */
int shell_execute(ShellVar *ptr)
{
	int (*builtin)(ShellVar *ptr);

	/*No argument provided*/
	if (!((*ptr).parsedArguments[0]))
	{
		return (1);
	}

	builtin = builtin_check((*ptr).parsedArguments[0]);

	if (builtin)
		return (builtin(ptr));

	return (exec_shell_cmd(ptr));
}
