SHELL=bash
NAME=container_test

CXX=clang++
CXXFLAGS=-Wall -Werror -Wextra -std=c++98

INC=$(addsuffix .hpp, enable_if, equal, integral_constant, is_integral, is_same, iterator_traits, lexicographical_compare, pair, rbtree, reverse_iterator, to_string, stack, vector, map)
SRC=main.cpp
OBJ=$(SRC:.cpp=.o)

all: $(NAME)

real_test: $(OBJ)
	$(CXX) $(CXXFLAGS) -D__STL__ $^ -o $@

test: $(NAME) real_test
	echo TEST; time ./container_test 28393 > /tmp/test.txt; echo REAL; time ./real_test 28393 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 21312 > /tmp/test.txt; echo REAL; time ./real_test 21312 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 25436 > /tmp/test.txt; echo REAL; time ./real_test 25436 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 26685 > /tmp/test.txt; echo REAL; time ./real_test 26685 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 13985 > /tmp/test.txt; echo REAL; time ./real_test 13985 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 49695 > /tmp/test.txt; echo REAL; time ./real_test 49695 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 82987 > /tmp/test.txt; echo REAL; time ./real_test 82987 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 97506 > /tmp/test.txt; echo REAL; time ./real_test 97506 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 89052 > /tmp/test.txt; echo REAL; time ./real_test 89052 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 38592 > /tmp/test.txt; echo REAL; time ./real_test 38592 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 29502 > /tmp/test.txt; echo REAL; time ./real_test 29502 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 84694 > /tmp/test.txt; echo REAL; time ./real_test 84694 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 89503 > /tmp/test.txt; echo REAL; time ./real_test 89503 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt
	echo TEST; time ./container_test 92843 > /tmp/test.txt; echo REAL; time ./real_test 92843 > /tmp/real.txt; diff /tmp/test.txt /tmp/real.txt

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp $(INC)
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
