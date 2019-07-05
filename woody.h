#ifndef __WOODY_H
# define __WOODY_H

# include <stdio.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <elf.h>
# include <errno.h>
# include <string.h>
# include "./libft/libft.h"

# define R8 0xb8
# define R9 0xb9

# define X(res) x(res, __FILE__, __LINE__);
# define XV(res) xv(res, __FILE__, __LINE__);

int		x(int res, char *file, int line);
void	*xv(void *res, char *file, int line);

#endif
