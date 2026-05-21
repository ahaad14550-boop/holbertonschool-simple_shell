#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 1024

/**
 * _getline - custom getline function using static buffer.
 * @lineptr: buffer that stores the input line.
 * @n: size of lineptr.
 * @stream: input stream (ignored but kept for prototype compatibility).
 * Return: number of bytes read, or -1 on failure/EOF.
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	static char buffer[BUF_SIZE];
	static size_t buf_pos;
	static size_t buf_size;
	size_t i = 0;
	char c;
	(void)stream;

	if (lineptr == NULL || n == NULL)
		return (-1);
	if (*lineptr == NULL || *n == 0)
	{
		*n = BUF_SIZE;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}
	while (1)
	{
		if (buf_pos >= buf_size)
		{
			buf_size = read(STDIN_FILENO, buffer, BUF_SIZE);
			buf_pos = 0;
			if (buf_size <= 0)
			{
				if (i == 0)
					return (-1);
				break;
			}
		}
		c = buffer[buf_pos++];
		if (i >= *n - 1)
		{
			*n += BUF_SIZE;
			*lineptr = realloc(*lineptr, *n);
			if (*lineptr == NULL)
				return (-1);
		}
		(*lineptr)[i++] = c;
		if (c == '\n')
			break;
	}
	(*lineptr)[i] = '\0';
	return (i);
}
