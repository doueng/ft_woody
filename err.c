#include "woody.h"

static void	err_exit(char *file, int line)
{
	printf("file: %s\nline: %d\n", file, line);
	perror(strerror(errno));
	exit(-1);
}

int			x(int res, char *file, int line)
{
	if (res == -1)
		err_exit(file, line);
	return (res);
}

void		*xv(void *res, char *file, int line)
{
	if (res == NULL)
		err_exit(file, line);
	return (res);
}
