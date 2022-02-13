#include <iostream>
#include "map.hpp"

template < class Map >
void	print(const Map &m) {
	typename Map::const_iterator it = m.begin();

	std::cout << "Map:" << std::endl;
	while (it != m.end())
		std::cout << it->first << ',' << it++->second << std::endl;
	std::cout << std::endl;
}

int	main(void) {
	ft::pair< const int, int > arr[] = { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5} };
	ft::map<int, int> m(arr, arr + 5);

	print(m);
	ft::map<int, int> r(m.begin(), m.end());
	print(r);
}
