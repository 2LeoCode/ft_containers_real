/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/28 22:04:09 by crochu            #+#    #+#             */
/*   Updated: 2022/02/16 14:45:31 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <list>
#include <cstdlib>

#include "stack.hpp"
#include "map.hpp"
#include "vector.hpp"

#ifdef __STL__
# define ft std
#endif

#define COUNT 100000

long					arr[COUNT];
ft::pair< long, long >	pair_arr[COUNT];

template < class T, class C >
void	print_stack(ft::stack< T, C > s) {
	std::cout << "size=" << s.size() << '\n';
	T	sum = T();
	while (!s.empty()) {
		sum += s.top();
		s.pop();
	}
	std::cout << sum << std::endl;
}

struct StackTest {
	void	stack(void) {
		{
			ft::stack< long >	s;
			print_stack(s);
		}
		{
			ft::stack< long, std::vector< long > >	s(
				std::vector< long >(COUNT, 42)
			);
			print_stack(s);
		}
		{
			std::vector< long >	v(COUNT);
			for (long i = 0; i < COUNT; ++i) {
				v.push_back(random());
			}
			ft::stack< int, std::vector< long > >	s(v);
			print_stack(s);
		}
	}
	void	assignation_operator(void) {
		{
			ft::stack< long, std::vector< long > >	s1, s2(
				std::vector< long >(COUNT, 42)
			);
			s2 = s1;
			print_stack(s2);
			s1 = ft::stack< long, std::vector< long > >(
				std::vector< long >(COUNT, 42)
			);
			print_stack(s1);
		}
	}
	void	push(void) {
		ft::stack< long, std::vector< long > >	s;

		for (int i = 0; i < COUNT; ++i)
			s.push(arr[i]);
		print_stack(s);
	}
	void	top(void) {
		ft::stack< long, std::vector< long > >	s;

		for (int i = 0; i < COUNT; ++i)
			s.push(arr[i]);
		std::cout << s.top() << std::endl;
		s.top() = 42;
		std::cout << s.top() << std::endl;
	}
};

template < class T >
void	print_vec(ft::vector< T > v) {
	std::cout <<
		"size:" << v.size() << '\n' <<
		"capacity:" << v.capacity()
	<< '\n';
	T	*data = v.data();
	T	sum = T();
	for (std::size_t i = 0; i < v.size(); ++i)
		sum += data[i];
	std::cout << sum << std::endl;
}

struct VectorTest {
	void	vector(void) {
		{
			ft::vector< long >	v;
			print_vec(v);
		}
		{
			ft::vector< long >	v(COUNT);
			print_vec(v);
		}
		{
			ft::vector< long >	v(static_cast< std::size_t >(COUNT), 42);
			print_vec(v);
		}
		{
			ft::vector< long >	v(COUNT, 42);
			print_vec(v);
		}
		{
			ft::vector< long >	v(arr, arr + COUNT);
			print_vec(v);
		}
		{
			ft::vector< long >	v(arr, arr + COUNT);
			ft::vector< long >	copy(v);
			print_vec(copy);
		}
	}
	void	assignation_operator(void) {
		ft::vector< long >	v(arr, arr + COUNT);
		ft::vector< long >	copy;
		copy = v;
		print_vec(copy);
	}
	void	assign(void) {
		{
			ft::vector< long >	v;
			v.assign(static_cast< std::size_t >(COUNT), 42);
			print_vec(v);
		}
		{
			ft::vector< long >	v;
			v.assign(COUNT, 42);
			print_vec(v);
		}
		{
			ft::vector< long >	v;
			v.assign(arr, arr + COUNT);
			print_vec(v);
		}
	}
	void	at(void) {
		ft::vector< long >	v(arr, arr + COUNT);
		try {
			v.at(-1);
		}
		catch (std::out_of_range &e) {
			std::cout << "OK" << std::endl;
		}
		try {
			v.at(COUNT);
		}
		catch (std::out_of_range &e) {
			std::cout << "OK" << std::endl;
		}
		std::cout << 
			v.at(COUNT - 1) << '\n' <<
			v.at(0) <<
		std::endl;
	}
	void	brackets_operator(void) {
		ft::vector< long >	v(COUNT);
		for (long i = 0; i < COUNT; ++i) {
			v[i] = random();
		}
		long	sum = 0;
		for (std::size_t i = 0; i < v.size(); ++i) {
			sum += v[i];
		}
		std::cout << sum << std::endl;
	}
	void	front(void) {
		ft::vector< long >	v(arr, arr + COUNT);
		std::cout << v.front() << '\n';
		v.front() = 42;
		std::cout << v.front() << std::endl;
	}
	void	back(void) {
		ft::vector< long >	v(arr, arr + COUNT);
		std::cout << v.back() << '\n';
		v.back() = 42;
		std::cout << v.back() << std::endl;
	}
	void	iterators(void) {
		ft::vector< long >	v(COUNT);
		for (ft::vector< long >::iterator it = v.begin(); it != v.end(); ++it)
			*it = random();
		long	sum = 0;
		for (ft::vector< long >::const_reverse_iterator it = v.rbegin(); it != v.rend() - 1; ++it) {
			sum += *it;
		}
		std::cout << sum << std::endl;
	}
	void	max_size(void) {
		std::cout << ft::vector< long >().max_size() << std::endl;
	}
	void	clear(void) {
		ft::vector< long >	v(arr, arr + COUNT);
		v.clear();
		std::cout << v.size() << " " << v.capacity() << std::endl;
	}
	void	insert(void) {
		{
			ft::vector< long >	v(arr, arr + COUNT / 2);
			v.insert(v.begin(), 42);
			print_vec(v);
		}
		{
			ft::vector< long >	v(arr, arr + COUNT / 2);
			v.insert(v.begin(), static_cast< std::size_t >(COUNT / 2), 42);
			print_vec(v);
		}
		{
			ft::vector< long >	v(arr, arr + COUNT / 2);
			v.insert(v.begin(), COUNT / 2, 42);
			print_vec(v);
		}
		{
			ft::vector< long >	v(arr, arr + COUNT / 2);
			v.insert(v.begin(), arr + COUNT / 2, arr + COUNT);
			print_vec(v);
		}
	}
	void	erase(void) {
		{
			ft::vector< long >	v(arr, arr + COUNT);
			ft::vector< long >::iterator	ret = v.erase(v.begin());
			std::cout << *ret << std::endl;
			print_vec(v);
		}
		{
			ft::vector< long >	v(arr, arr + COUNT);
			ft::vector< long >::iterator	ret = v.erase(v.begin(), v.begin() + COUNT / 2);
			std::cout << *ret << std::endl;
			print_vec(v);
		}
	}
	void	push_back(void) {
		ft::vector< long >	v;
		for (long i = 0; i < COUNT; ++i) v.push_back(arr[i]);
		print_vec(v);
	}
	void	pop_back(void) {
		ft::vector< long >	v(arr, arr + COUNT);
		while (!v.empty()) v.pop_back();
		print_vec(v);
	}
	void	resize(void) {
		{
			ft::vector< long >	v(arr, arr + COUNT);
			v.resize(COUNT / 2);
			print_vec(v);
		}
		{
			ft::vector< long >	v(arr, arr + COUNT / 2);
			v.resize(COUNT);
			print_vec(v);
		}
	}
	void	swap(void) {
		ft::vector< long >	a(COUNT, 42);
		ft::vector< long >	b(arr, arr + COUNT);
		a.swap(b);
		print_vec(a), print_vec(b);
	}
};
template < class Key, class T, class Compare >
void	print_map(ft::map< Key, T, Compare > m) {
	std::cout <<
		"size=" << m.size() << '\n';
	T	sum = 0;
	for (typename ft::map< Key, T, Compare >::const_iterator it = m.begin(); it != m.end(); ++it) {
		sum += it->second;
	}
	std::cout << sum << std::endl;
}

template < class T >
struct Greater : public std::binary_function< T, T, bool > {
	bool	operator ()(const T &x, const T &y) const {
		return x > y;
	}
};

struct MapTest {
	void	map(void) {
		{
			ft::map< long, long >	m;
			print_map(m);
		}
		{
			Greater< long >	g;
			ft::map< long, long, std::binary_function< long, long, bool > >	m(g);
			print_map(m);
		}
		{
			ft::map< long, long >	m(pair_arr, pair_arr + COUNT);
			print_map(m);
			ft::map< long, long >	copy(m);
			print_map(copy);
		}
	}
	void	assignation_operator(void) {
		ft::map< long, long >	m(pair_arr, pair_arr + COUNT);
		ft::map< long, long >	copy;
		copy = m;
		print_map(copy);
	}
	void	at(void) {
		ft::map< long, long >	m(pair_arr, pair_arr + COUNT);
		try {
			while (true) m.at(random());
		} catch (const std::out_of_range &e) {
			std::cout << e.what() << std::endl;
		}
		m.at(m.begin()->first) = 0;
		print_map(m);
	}
	void	brackets_operator(void) {
		ft::map< long, long >	m(pair_arr, pair_arr + COUNT);
		m[m.begin()->first] = 0;
		long	index;
		try {
			while (true) m.at(index = random());
		} catch (std::out_of_range &e) {
			m[index];
			try {
				m.at(index);
			} catch (...) {
				std::cout << "KO" << std::endl;
			}
			m[index] = std::numeric_limits< long >::min();
		}
		print_map(m);
	}
	void	iterators(void) {
		ft::map< long, long >	m(pair_arr, pair_arr + COUNT);
		ft::map< long, long >::iterator	it = m.begin();
		for (int i = 0; i < COUNT / 2; ++i)
			it++->second = 0;
		ft::map< long, long >::const_iterator	cit = m.begin();
		for (int i = 0; i < COUNT / 2; ++i)
			if (cit->second) {
				std::cout << "KO" << std::endl;
				break ;
			}
		ft::map< long, long >::const_reverse_iterator	crit = m.rbegin();
		long	sum = 0;
		for (int i = 0; i < COUNT / 2; ++i)
			sum += crit++->second;
		std::cout << sum << std::endl;
	}
	void	max_size(void) {
		std::cout << ft::map< long, long >().max_size() << std::endl;
	}
	void	insert(void) {
		ft::map< long, long >	m;
		for (int i = 0; i < COUNT / 2; ++i) m.insert(pair_arr[i]);
		print_map(m);
		m.insert(pair_arr + COUNT / 2, pair_arr + COUNT);
		print_map(m);
	}
	void	erase(void) {
		ft::map< long, long >	m(pair_arr, pair_arr + COUNT);
		m.erase(m.begin());
		while (m.erase(random()));
		print_map(m);

		ft::map< long, long >::iterator	it1, it2;
		it1 = it2 = m.begin();
		for (int i = 0; i < COUNT / 2; ++i) ++it1;
		for (int i = 0; i < COUNT / 4; ++i) ++it2;
		m.erase(it2, it1);
		print_map(m);

		m.erase(m.begin(), m.end());
		print_map(m);
	}
	void	swap(void) {
		ft::map< long, long >	m(pair_arr, pair_arr + COUNT);
		ft::map< long, long >	swappy;
		swappy.insert(ft::make_pair(42, 42));
		swappy.swap(m);
		print_map(m);
		print_map(swappy);
	}
	void	find(void) {
		ft::map< long, long >	m(pair_arr, pair_arr + COUNT);
		ft::map< long, long >::iterator	it;
		while ((it = m.find(random())) == m.end());
		std::cout << it->second << std::endl;
		it->second = std::numeric_limits< long >::min();
		print_map(m);
	}
};
template < class MAP >
void	print_map(const MAP &m) {
	std::cout << "--->map<---" << std::endl;
	for (typename MAP::const_iterator it = m.begin(); it != m.end(); ++it) {
		std::cout << it->first << " --> " << it->second << std::endl;
	}
}

void init_tests(int seed) {
	srandom(seed);
	for (int i = 0; i < COUNT; ++i) {
		arr[i] = random();
		pair_arr[i].first = random();
		pair_arr[i].second = random();
	}
}

int	main(int argc, char **argv) {
	if (argc != 2)
		std::cerr << "Usage: " << *argv << " <seed>" << std::endl;
	init_tests(atol(argv[1]));

	StackTest	s_test;
	VectorTest	v_test;
	MapTest		m_test;

	s_test.stack();
	s_test.assignation_operator();
	s_test.push();
	s_test.top();

	v_test.vector();
	v_test.assignation_operator();
	v_test.assign();
	v_test.at();
	v_test.brackets_operator();
	v_test.front();
	v_test.back();
	v_test.erase();
	v_test.push_back();
	v_test.pop_back();
	v_test.resize();
	v_test.swap();

	m_test.map();
	m_test.assignation_operator();
	m_test.at();
	m_test.brackets_operator();
	m_test.max_size();
	m_test.insert();
	m_test.erase();
	m_test.swap();
	m_test.find();
	return 0;
}
