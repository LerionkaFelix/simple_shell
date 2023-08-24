#include "shell.h"

/**
 * dptr_realloc_sh - performs double pointer memory block reallocation
 * @dptr: double pointer to the previously allocated memory block.
 * @dptr_size: size of the previously allocated memory block in bytes
 * @new_size: size of the new memory block.
 * Return: dptr if new_size is equal to dptr_size, or pointer to the newly allocated
 *         memory block if successfull, otherwise NULL
 */
char **dptr_realloc_sh(char **dptr, unsigned int dptr_size, unsigned int new_size)
{
	char **new_ptr;
	unsigned int idx;

	if (new_size == dptr_size)
		return (dptr);
	
	if (!dptr)
		return (malloc(sizeof(char *) * new_size));

	new_ptr = malloc(sizeof(char *) * new_size);
	
	if (!new_ptr)
		return (NULL);
	
	for (idx = 0; idx < dptr_size; idx++)
		new_ptr[idx] = dptr[idx];

	free(dptr);

	return (new_ptr);
}

/**
 * mem_realloc - reallocates memory to a new memory block 
 * @optr: pointer to current memory block
 * @osize: size of current memory block
 * @nsize: size of new memory block
 * Return: NULL or pointer to the new memory block
 */

void *mem_realloc(void *optr, size_t osize, size_t nsize)
{
	void *new_block;

	if (nsize == 0)
	{
		free(optr);
		return (NULL);
	}

	if (!optr)
	{
		return (malloc(nsize));
	}

	if (nsize <= osize)
	{
		return (optr);
	}

	new_block = malloc(nsize);
	if (new_block)
	{
		sh_memcpy(new_block, optr, osize);
		free(optr);
	}

	return (new_block);
}

/**
 * str_reallocate - allocate memory for an array of strings
 * @strarr_ptr: pointer to an array of strings
 * @osize: old array size
 * @nsize: new array size
 * Return: pointer to the newly allocated memory block
 */
char **str_reallocate(char **strarr_ptr, unsigned int osize, unsigned int nsize)
{
	char **new_block;
	unsigned int idx;

	if (!strarr_ptr)
		return (malloc(sizeof(char *) * nsize));

	if (nsize == osize)
		return (strarr_ptr);

	new_block = malloc(sizeof(char *) * nsize);

	if (!new_block)
		return (NULL);

	if (nsize < osize)
		osize = nsize;

	for (idx = 0; idx < osize; idx++)
		new_block[idx] = strarr_ptr[idx];

	free(strarr_ptr);
	return (new_block);
}
