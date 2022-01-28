NAME=container_test

CC=clang++
CFLAGS=-Wall -Werror -Wextra -std=c++98

INC=$(addsuffix .hpp, enable_if, equal, integral_constant, is_integral, is_same, iterator_traits, lexicographical_compare, pair, rbtree, reverse_iterator, to_string, stack, vector, map)
SRC=main.c
OBJ=$(SRC:.c=.o)

all: $(NAME)

test: $(NAME)
	./$<

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
