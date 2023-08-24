#include "shell.h"

/**
 * create_new_envvar - create new environ key-value pair
 * @key: environ key
 * @val: environ value
 * Return: new environ variable
 */
char *create_new_envvar(char *key, char *val)
{
	int len = sh_strlen(key) + sh_strlen(val) + 2;
	char *env_var = malloc(sizeof(char) * len);

	if (!env_var)
		return (NULL);

	sprintf(env_var, "%s=%s", key, val);
	return (env_var);
}

/**
 * set_envname - resets the name of the environ variable
 * @ptr: pointer to ShellVar variable
 * Return: 1 once the name is set
 */
int set_envname(ShellVar *ptr)
{
	char *env_name, *envvar;
	char *key, *value, **env;
	int idx;

	if (!(ptr->parsedArguments[1]) || !(ptr->parsedArguments[2]))
	{
		error_handler(ptr, -1);
		return (1);
	}

	key = ptr->parsedArguments[1];
	value = ptr->parsedArguments[2];

	for (idx = 0; ptr->environ_vars[idx]; idx++)
	{
		envvar = sh_strdup(ptr->environ_vars[idx]);
		env_name = str_tknzr(envvar, "=");
		if (sh_strcmp(env_name, key) == 0)
		{
			free(ptr->environ_vars[idx]);
			ptr->environ_vars[idx] = create_new_envvar(env_name, value);
			free(envvar);
			return (1);
		}
		free(envvar);
	}

	env = ptr->environ_vars;
	env = dptr_realloc_sh(env, idx, sizeof(char *) * (idx + 2));
	env[idx] = create_new_envvar(key, value);
	env[idx + 1] = NULL;
	ptr->environ_vars = env;
	return (1);
}

/**
 * unset_envvar - unsets an environ variable
 * @ptr: pointer to ShellVar variable
 * Return: 1 on successful removal
 */
int unset_envvar(ShellVar *ptr)
{
	char **new_envvars;
	char *var, *name;
	int idx1, idx2, idx3;

	if (!(ptr->parsedArguments[1]))
	{
		error_handler(ptr, -1);
		return (1);
	}

	idx3 = -1, idx1 = 0;
	while (ptr->environ_vars[idx1])
	{
		var = sh_strdup(ptr->environ_vars[idx1]);
		name = str_tknzr(var, "=");
		if (sh_strcmp(name, ptr->parsedArguments[1]) == 0)
			idx3 = idx1;
		free(var);
		idx1++;
	}
	
	if (idx3 == -1)
	{
		error_handler(ptr, -1);
		return (1);
	}
	
	new_envvars = malloc(sizeof(char *) * idx1);
	
	idx1 = idx2 = 0;
	while (ptr->environ_vars[idx1])
	{
		if (idx1 != idx2)
		{
			new_envvars[idx2] = ptr->environ_vars[idx1];
			idx2++;
		}
		idx1++;
	}

	new_envvars[idx2] = NULL;
	
	free(ptr->environ_vars[idx3]);
	free(ptr->environ_vars);
	
	ptr->environ_vars = new_envvars;
	return (1);
}

/**
 * get_envname - gets the value of the requested environment variable
 * @env_name: name of the environ variable
 * @env_arr: pointer to the environ variables array
 * Return: pointer to the value of the specified environ variable, NULL otherwise
 */
char *get_envname(const char *env_name, char **env_arr)
{
	int idx, idx1;
	char *envptr;

	envptr = NULL;
	idx1 = 0;

	for (idx = 0; env_arr[idx]; idx++)
	{
		idx1 = 0;
		while (env_arr[idx][idx1] == env_name[idx1] && env_name[idx1] != '\0')
			idx1++;
		
		if (env_arr[idx][idx1] == '=' && env_name[idx1] == '\0')
		{
			envptr = &env_arr[idx][idx1 + 1];
			break;
		}
	}

	return (envptr);
}


/**
 * set_envvar - sets an environment variable to environ array
 * @ptr: pointer to ShellVar variable
 * @env_name: name of the environ variable to be set
 * @env_val: value to be set for the provided variable name
 * Return: void
 */
void set_envvar(char *env_name, char *env_val, ShellVar *ptr)
{
	char **env;
	char *current_key, *current_val;
	int arr_len, idx;

	arr_len = 0;
	while ((*ptr).environ_vars[arr_len])
	{
		arr_len++;
	}

	for (idx = 0; idx < arr_len; idx++)
	{
		current_val = sh_strdup((*ptr).environ_vars[idx]);
		current_key = str_tknzr(current_val, "=");

		if (sh_strcmp(current_key, env_name) == 0)
		{
			free((*ptr).environ_vars[idx]);
			(*ptr).environ_vars[idx] = create_new_envvar(current_key, env_val);
			free(current_val);
			return;
		}

		free(current_val);
	}

	env = (*ptr).environ_vars;
	env = str_reallocate(env, arr_len, sizeof(char *) * (arr_len + 2));
	env[arr_len] = create_new_envvar(env_name, env_val);
	env[arr_len + 1] = NULL;
	(*ptr).environ_vars = env;
}
