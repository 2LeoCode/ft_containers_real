SHELL=bash
NAME=containers_test

CXX=clang++
CXXFLAGS=-MMD -Wall -Werror -Wextra -std=c++98

SRC=main.cpp
OBJ=$(SRC:.cpp=.o)
DEP=$(OBJ:.o=.d)

all: $(NAME)

-include $(DEP)

containers_real: $(OBJ)
	$(CXX) $(CXXFLAGS) -D__STL__ $^ -o $@

test: $(NAME) containers_real
	@printf '==========\nTEST\n'; time ./containers_test 28393 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 28393 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 21312 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 21312 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 25436 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 25436 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 26685 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 26685 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 13985 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 13985 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 49695 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 49695 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 82987 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 82987 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 97506 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 97506 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 89052 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 89052 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 38592 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 38592 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 29502 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 29502 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 84694 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 84694 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 89503 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 89503 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo
	@printf '==========\nTEST\n'; time ./containers_test 92843 > /tmp/test.txt; printf '\n\nREAL\n'; time ./containers_real 92843 > /tmp/real.txt; printf '\n\nDIFF\n'; diff /tmp/test.txt /tmp/real.txt; echo

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -f $(NAME) containers_real

re: fclean all

.PHONY: all clean fclean re
