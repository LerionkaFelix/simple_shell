#include "shell.h"

/**
 * change_destdir - change the current filepath to the specified directory.
 * @ptr: pointer to ShellVar struct.
 * Return: void
 */
void change_destdir(ShellVar *ptr)
{
	char *dest_dir, *temp_currdir, *temp_destdir;
	char curr_dir[PATH_MAX];

	getcwd(curr_dir, sizeof(curr_dir));

	dest_dir = (*ptr).parsedArguments[1];

	if (chdir(dest_dir) == -1)
	{
		error_handler(ptr, 2);
		return;
	}

	temp_currdir = sh_strdup(curr_dir);
	set_envvar("OLDPWD", temp_currdir, ptr);
	free(temp_currdir);

	/* Update the PWD environment variable.*/
	temp_destdir = sh_strdup(dest_dir);
	set_envvar("PWD", temp_destdir, ptr);
	free(temp_destdir);

	(*ptr).exit_code = 0;
	chdir(dest_dir);
}

/**
 * change_homedir - switches to the system's home directory
 * @ptr: pointer to ShellVar variable
 * Return: void
 */
void change_homedir(ShellVar *ptr)
{
	char pr_wd[PATH_MAX];
	char *home_dir, *prev_wd;
	
	getcwd(pr_wd, sizeof(pr_wd));
	prev_wd = sh_strdup(pr_wd);

	home_dir = get_envname("HOME", (*ptr).environ_vars);

	if (!home_dir)
	{
		set_envvar("OLDPWD", prev_wd, ptr);
		free(prev_wd);
		return;
	}

	if (chdir(home_dir) == -1)
	{
		error_handler(ptr, 2);
		free(prev_wd);
		return;
	}

	set_envvar("OLDPWD", prev_wd, ptr);
	set_envvar("PWD", home_dir, ptr);
	free(prev_wd);
	(*ptr).exit_code  = 0;
}

/**
 * change_pardir - change the current path to the parent directory.
 * @ptr: pointer to the ShellVar struct.
 * Return: void
 */
void change_pardir(ShellVar *ptr)
{
	char *dir, *temp_cwd, *temp_pwd;
	char curr_dir[PATH_MAX];

	getcwd(curr_dir, sizeof(curr_dir));
	temp_cwd = sh_strdup(curr_dir);
	set_envvar("OLDPWD", temp_cwd, ptr);
	dir = ptr->parsedArguments[1];
	
	if (sh_strcmp(".", dir) == 0)
	{
		set_envvar("PWD", temp_cwd, ptr);
		free(temp_cwd);
		return;
	}
	
	if (sh_strcmp("/", temp_cwd) == 0)
	{
		free(temp_cwd);
		return;
	}
	
	temp_pwd = temp_cwd;
	sh_strrev(temp_pwd);
	
	temp_pwd = str_tknzr(temp_pwd, "/");
	if (temp_pwd)
	{
		temp_pwd = str_tknzr(NULL, "\0");

		if (temp_pwd)
			sh_strrev(temp_pwd);
	}
	
	if (temp_pwd)
		chdir(temp_pwd), set_envvar("PWD", temp_pwd, ptr);
	else
		chdir("/"), set_envvar("PWD", "/", ptr);

	(*ptr).exit_code = 0;
	free(temp_cwd);
}


/**
 * change_upperdir - switches to the previous directory in the directory tree
 * @ptr: pointer to ShellVar variable
 * Return: void
 */
void change_upperdir(ShellVar *ptr)
{
	char curr_wd[PATH_MAX];
	char *opwd, *npwd, *opwd_val;

	if (!(getcwd(curr_wd, sizeof(curr_wd))))
	{
		perror("getcwd() error!");
		return;
	}

	opwd_val = get_envname("OLDPWD", (*ptr).environ_vars);

	if (!opwd_val)
	{
		dprintf(STDERR_FILENO, "OLDPWD not set!\n");
		return;
	}

	opwd = sh_strdup(curr_wd);

	if (chdir(opwd_val) == -1)
	{
		perror("chdir() error");
		free(opwd);
		return;
	}

	npwd = sh_strdup(opwd_val);
	set_envvar("PWD", npwd, ptr);
	set_envvar("OLDPWD", opwd, ptr);

	printf("%s\n", npwd);

	free(opwd);
	free(npwd);

	ptr->exit_code = 0;
}

/**
 * change_argsdir - switches directory based on the specified command
 *                    line arguments 
 * @ptr: pointer to ShellVar variable
 * Return: 1
 */
int change_argsdir(ShellVar *ptr)
{
	int idx;
	char *arg;

	if (!((*ptr).parsedArguments[1]))
	{
		change_homedir(ptr);
		return (1);
	}

	for (idx = 1; (*ptr).parsedArguments[idx]; idx++)
	{
		arg = (*ptr).parsedArguments[idx];

		if (sh_strcmp("$HOME", arg) == 0)
		{
			change_homedir(ptr);
			continue;
		}
		if (sh_strcmp("~", arg) == 0 || sh_strcmp("--", arg) == 0)
		{
			change_homedir(ptr);
			continue;
		}
		if (sh_strcmp("-", arg) == 0)
		{
			change_upperdir(ptr);
			continue;
		}
		if (sh_strcmp(".", arg) == 0 || sh_strcmp("..", arg) == 0)
		{
			change_pardir(ptr);
			continue;
		}

		change_destdir(ptr);
	}
	return (1);
}
