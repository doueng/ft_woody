NAME = ./pecker

LIB = ./libft/libft.a
LIBFT = ./libft

FLAGS = -Werror -Wextra -Wall -g

CC = gcc

SRC = $(wildcard *.c)

SRCO = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(SRCO)
	make -C $(LIBFT)
	$(CC) $(SRCO) $(FLAGS) $(LIB) -o $(NAME)

%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@

clean:
	/bin/rm -f $(SRCO)

fclean: clean
	/bin/rm -f $(NAME)
	/bin/rm woody

.SILENT:
re: fclean all
