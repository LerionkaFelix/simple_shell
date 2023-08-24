#include "shell.h"

/**
 * sh_strlen - returns length of a string typed within the shell program
 * @str: string
 * Return: length of s
 */

int sh_strlen(const char *str)
{
	int i = 0;

	while (str[++i])
		;
	return (i);
}

/**
 * sh_strdup - returns a pointer to a newly duplicated string
 * @str: string to be duplicated
 * Return: pointer to the memory alllocated for the duplicated string
 */
char *sh_strdup(char *str)
{
	size_t s_len = sh_strlen(str) + 1;
	char *dupstr = malloc(s_len * sizeof(char));

	if (str == NULL)
		return (NULL);

	if (dupstr == NULL)
		return (NULL);

	sh_memcpy(dupstr, str, s_len);

	return (dupstr);
}

/**
 * sh_strncpy - function that copies a string
 * @src: source string to be copied
 * @dest: destination string to copy to
 * @n: number of bytes to copy
 * Return: pointer to the copied string
 */
char *sh_strncpy(char *dest, char *src, int n)
{
	int idx;

	for (idx = 0; idx < n; idx++)
		dest[idx] = src[idx];

	for (; idx < n; idx++)
		dest[idx] = '\0';

	return (dest);
}

/**
 * sh_strchr - searches for a character in string
 * @str: pointer to the string to be traversed
 * @c: character to be searched
 * Return: pointer to the character's location in the string, 
 *         NULL otherwise
 */
char *sh_strchr(char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (str);
		str++;
	}

	return (NULL);
}

/**
*str_tknzr - tokenizes a string
*@str: pointer to a string
*@delim: pointer to array of delimiter characters
*Return: pointer to a token if str isn't empty, otherwise NULL
*/
char *str_tknzr(char *str, const char *dlmtr)
{
	char *tkn_start, *tkn_end;
	static char *last;

	if (!str)
		str = last;

	str += sh_strspn(str, dlmtr);

	if (*str == '\0')
		return (NULL);

	tkn_start = str;
	tkn_end = sh_strbytematch(tkn_start, dlmtr);

	if (tkn_end)
	{
		*tkn_end = '\0';
		last = tkn_end + 1;
	}
	else
	{
		last = tkn_start + sh_strlen(tkn_start);
	}

	return (tkn_start);
}

/**
 * sh_strndup - allocates memory of a specific size for a copy 
 *              of a string passed as an argument 
 * @str: string to be copied
 * @i: number of bytes to copy
 * Return: a pointer to the newly allocated memory space for the copy
 *         if str isn't empty, otherwise NULL
 */
char *sh_strndup(char *str, int i)
{
	char *dup;
	int idx;
	int length = sh_strlen(str);

	if (!str)
		return (NULL);

	if (length < i)
		i = length;

	dup = malloc(i + 1);

	if (!dup)
		return (NULL);

	for (idx = 0; idx < i; idx++)
		dup[idx] = str[idx];

	dup[idx] = '\0';
	return (dup);
}

/**
 * get_num_len - returns length of a number typed within the shell program
 * @num: integer
 * Return: len
 */
int get_num_len(int num)
{
	int len = 0;
	
	if (num == 0)
		return (1);

	if (num < 0)
	{
		len++;
		num = -(num);
	}

	while (num)
	{
		len++;
		num /= 10;
	}

	return (len);
}

/**
 * sh_itoa - converts an integer to a string
 * @n: number to convert
 * Return: string that represents the number
 */
char *sh_itoa(int n)
{
	char *buff;
	unsigned int abs_n = (n < 0) ? -n : n;
	int idx, length = get_num_len(abs_n);

	if (n < 0)
		length++;

	buff = (char *) malloc(length + 1);

	if (!buff)
	{
		return (NULL);
	}
	buff[length] = '\0';

	idx = length - 1;

	do {
		buff[idx--] = (abs_n % 10) + '0';
		abs_n /= 10;
	} while (abs_n > 0);

	if (n < 0)
	{
		buff[0] = '-';
	}

	return (buff);
}

/**
 * sh_isdigit- checks if a string is a digit
 * @s: string to be traversed
 * Return: 1 if success, otherwise 0
 */
int sh_isdigit(const char *s)
{
	int idx = 0;

	while (s[idx] != '\0')
	{
		if (!(s[idx] >= '0' && s[idx] <= '9'))
		{
			return (0);
		}
		idx++;
	}
	return (1);
}

/**
 * sh_atoi - performs string to integer conversion
 * @str: string to convert
 * Return: resulting integer
 */
int sh_atoi(const char *str)
{
	int idx = 0;
	int sign = 1;
	int intres = 0;

	if (str[idx] == '-')
	{
		sign = -1;
		idx++;
	}
	else if (str[idx] == '+')
	{
		idx++;
	}

	while (str[idx] != '\0')
	{
		if (str[idx] >= '0' && str[idx] <= '9')
		{
			intres = intres * 10 + (str[idx] - '0');
			idx++;
		}
		else
		{
			break;
		}
	}

	return (sign * intres);
}

/**
 * sh_strcmp - Compare two strings for equality
 * @str1: pointer to the first string.
 * @str2: pointer to the second string.
 * Return: 0 if they are equal, negative number if str1 is less than str2, or a
 *         positive number if str1 is greater than str2.
 */
int sh_strcmp(const char *str1, const char *str2)
{
	while (*str1 && *str2 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}

	return (*str1 - *str2);
}

/**
 * sh_strcpy - copies one string to another string
 * @src: string to be copied
 * @dest: destination to copy
 * Return: dest
 */
char *sh_strcpy(char *dest, const char *src)
{
	int idx = 0;

	while (src[idx] != '\0')
	{
		dest[idx] = src[idx];
		idx++;
	}
	dest[idx] = '\0';
	return (dest);
}

/**
 * sh_strrev - reverses a string
 * @str: string to be reversed
 */
void sh_strrev(char *str)
{
	char temp, *s;
	int idx = 0, x, y;

	while (str[idx] != '\0')
		idx++;

	s = str;

	for (x = 0, y = idx - 1; x < y; x++, y--)
	{
		temp = *(s + x);
		*(s + x) = *(s + y);
		*(s + y) = temp;
	}
}

/**
 * sh_strspn - calculates the length of the initial segment of str
 *             which only contains characters in match.
 * @s: string to be traversed.
 * @match: string containing the characters to match.
 * Return: number of bytes in the initial segment of str that
 *         consists of characters in match.
 */
unsigned int sh_strspn(char *str, const char *match)
{
	unsigned int idx1, idx2, is_match;

	for (idx1 = 0; str[idx1]; idx1++)
	{
		is_match = 0;
		for (idx2 = 0; match[idx2]; idx2++)
		{
			if (str[idx1] == match[idx2])
			{
				is_match = 1;
				break;
			}
		}
		if (!is_match)
			break;
	}
	return (idx1);
}

/**
 * sh_strbytematch - searches a string for any characters that 
 *              are contained in a set of characters to be matched
 * @str: string to be traversed
 * @charset: string containing the characters to be matched.
 * Return: pointer to the byte in str that matches one of the bytes in charset, 
 *         or NULL if no match is found.
 */
char *sh_strbytematch(char *str, const char *charset)
{
	unsigned int i, j;

	for (i = 0; str[i]; i++)
	{
		for (j = 0; charset[j]; j++)
		{
			if (str[i] == charset[j])
				return (&str[i]);
		}
	}
	return (NULL);
}

/**
 * sh_memcpy - copies memory area to be filled with characters
 * @dest: memory area to be filled
 * @src: string to be copied
 * @n: number of characters to copy
 * Return: dest memory location
 */
char *sh_memcpy(char *dest, char *src, unsigned int n)
{
	unsigned int idx = 0;

	while (idx < n - 1)
	{
		dest[idx] = src[idx];
		idx++;
	}

	dest[idx] = '\0';
	return (dest);
}