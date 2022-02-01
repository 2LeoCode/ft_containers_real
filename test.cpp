#include "map.hpp"

int main(void) {
	ft::map<int, int> m;
	m.insert(ft::make_pair(5, 5));
	m.erase(m.end());
	return 0;
}
