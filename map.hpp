/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 19:06:26 by crochu            #+#    #+#             */
/*   Updated: 2022/02/01 21:09:00 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rbtree.hpp"
#include "pair.hpp"
#include "equal.hpp"
#include "lexicographical_compare.hpp"
#include <functional>
#include <algorithm>

namespace ft {
	template <
		class Key,
		class T,
		class Compare = std::less< Key >,
		class Allocator = std::allocator< ft::pair< const Key, T > >
	> class map {
		public:
			class value_compare;

			typedef Key										key_type;
			typedef T										mapped_type;
			typedef ft::pair< const Key, T >				value_type;
			typedef std::size_t								size_type;
			typedef std::ptrdiff_t							difference_type;
			typedef Compare									key_compare;
			typedef Allocator								allocator_type;
			typedef value_type&								reference;
			typedef const value_type&						const_reference;
			typedef typename Allocator::pointer				pointer;
			typedef typename Allocator::const_pointer		const_pointer;
			typedef typename rbtree<
				value_type, value_compare, Allocator, size_type, difference_type
			>::iterator										iterator;
			typedef typename rbtree<
				value_type, value_compare, Allocator, size_type, difference_type
			>::const_iterator								const_iterator;
			typedef ft::reverse_iterator< iterator >		reverse_iterator;
			typedef ft::reverse_iterator< const_iterator >	const_reverse_iterator;

			class value_compare
			:	public std::binary_function< value_type, value_type, bool > {
				friend class map;
				public:
					bool	operator ()(
						const value_type &lhs,
						const value_type &rhs
					) const { return comp(lhs.first, rhs.first); }
				
				protected:
					value_compare(Compare c) : comp(c) { }
					Compare	comp;
			};


			map() : m_key_comp(), m_value_comp(m_key_comp), m_alloc(), m_tree(m_value_comp) { }
			explicit map(
				const Compare &comp,
				const Allocator &alloc = Allocator()
			) :	m_key_comp(comp),
				m_value_comp(comp),
				m_alloc(alloc),
				m_tree(m_value_comp, alloc)
			{ }
			template <
				class InputIt
			> map(
				InputIt first, InputIt last,
				const Compare& comp = Compare(),
				const Allocator& alloc = Allocator()
			) :	m_key_comp(comp),
				m_value_comp(comp),
				m_alloc(alloc),
				m_tree(first, last, m_value_comp, alloc)
			{ }
			map(const map &other)
			:	m_key_comp(other.m_key_comp),
				m_value_comp(other.m_key_comp),
				m_alloc(other.m_alloc),
				m_tree(other.m_tree)
			{ }
			~map() { clear(); }

			map	&operator =(const map &other) {
				m_key_comp = other.m_key_comp,
				m_value_comp = other.m_value_comp,
				m_alloc = other.m_alloc,
				m_tree = other.m_tree;
				return *this;
			}

			allocator_type	get_allocator() const { return m_alloc; }

			T				&at(const Key &key) {
				return m_tree.find(value_type(key, T())).second;
			}
			const T			&at(const Key &key) const {
				return m_tree.find(value_type(key, T())).second;
			}
			T				&operator [](const Key &key) {
				ft::pair< iterator, bool > value;
				try {
					return at(key);
				} catch (...) { }
				return m_tree.insert(value_type(key, T())).first->second;
			}

			iterator				begin(void) { return m_tree.begin(); }
			const_iterator			begin(void) const { return m_tree.begin(); }
			iterator				end(void) { return m_tree.end(); }
			const_iterator			end(void) const { return m_tree.end(); }
			reverse_iterator		rbegin(void) { return m_tree.rbegin(); }
			const_reverse_iterator	rbegin(void) const { return m_tree.rbegin(); }
			reverse_iterator		rend(void) { return m_tree.rend(); }
			const_reverse_iterator	rend(void) const { return m_tree.rend(); }

			bool					empty(void) const { return !m_tree.size(); }
			size_type				size(void) const { return m_tree.size(); }
			size_type				max_size(void) const { return m_tree.max_size(); }

			void					clear(void) { m_tree.clear(); }
			ft::pair<
				iterator,
				bool
			>						insert(const value_type &value) {
				return m_tree.insert(value);
			}
			iterator				insert(iterator hint, const value_type &value) {
				if (hint == end())
					return m_tree.insert(value).first;
				return m_tree.insert(value, reinterpret_cast< void* >(const_cast< value_type* >(&*hint))).first;
			}
			template <
				class InputIt
			> void					insert(InputIt first, InputIt last) {
				while (first != last)
					m_tree.insert(*first++);
			}
			void					erase(iterator pos) {
				m_tree.remove(*pos);
			}
			void					erase(iterator first, iterator last) {
				m_tree.remove(first, last);
			}
			size_type				erase(const Key &key) {
				try {
					m_tree.remove(value_type(key, T()));
				} catch (...) { return 0; }
				return 1;
			}
			void					swap(map &other) {
				std::swap(m_key_comp, other.m_key_comp);
				std::swap(m_value_comp, other.m_value_comp);
				std::swap(m_alloc, other.m_alloc);
				m_tree.swap(other.m_tree);
			}

			size_type				count(const Key &key) const {
				try {
					m_tree.find(value_type(key, T()));
				} catch (...) { return 0; }
				return 1;
			}
			iterator				find(const Key &key) {
				try {
					return m_tree.find_iterator(value_type(key, T()));
				} catch (...) { }
				return end();
			}
			const_iterator			find(const Key &key) const {
				try {
					return m_tree.find_iterator(value_type(key, T()));
				} catch (...) { }
				return end();
			}
			ft::pair<
				iterator,
				iterator
			>						equal_range(const Key &key) {
				return ft::make_pair(lower_bound(key), upper_bound(key));
			}
			ft::pair<
				const_iterator,
				const_iterator
			>						equal_range(const Key &key) const {
				return ft::make_pair(lower_bound(key), upper_bound(key));
			}
			iterator				lower_bound(const Key &key) {
				return m_tree.lower_bound(value_type(key, T()));
			}
			const_iterator			lower_bound(const Key &key) const {
				return m_tree.lower_bound(value_type(key, T()));
			}
			iterator				upper_bound(const Key &key) {
				return m_tree.upper_bound(value_type(key, T()));
			}
			const_iterator			upper_bound(const Key &key) const {
				return m_tree.upper_bound(value_type(key, T()));
			}
			
			key_compare				key_comp(void) const { return m_key_comp; }
			value_compare			value_comp(void) const { return m_value_comp; }			

			// TEST

			//void					generate_dot(const char *name) const {
			//	m_tree.generate_dot(name);
			//}
			
		private:
			Compare			m_key_comp;
			value_compare	m_value_comp;
			Allocator		m_alloc;
			rbtree<
				value_type,
				value_compare,
				Allocator,
				size_type,
				difference_type
			>				m_tree;
	};

	template <
		class Key,
		class T,
		class Compare,
		class Alloc
	> void	swap(
		ft::map< Key, T, Compare, Alloc > &lhs,
		ft::map< Key, T, Compare, Alloc > &rhs
	) { lhs.swap(rhs); }
}

template <
	class Key,
	class T,
	class Compare,
	class Alloc
> bool	operator ==(
	const ft::map< Key, T, Compare, Alloc > &lhs,
	const ft::map< Key, T, Compare, Alloc > &rhs
) {
	return (
		lhs.size() == rhs.size()
		&& ft::equal(lhs.begin(), lhs.end(), rhs.begin())
	);
}

template <
	class Key,
	class T,
	class Compare,
	class Alloc
> bool	operator <(
	const ft::map< Key, T, Compare, Alloc > &lhs,
	const ft::map< Key, T, Compare, Alloc > &rhs
) {
	return ft::lexicographical_compare(
		lhs.begin(), lhs.end(),
		rhs.begin(), rhs.end()
	);
}

template <
	class Key,
	class T,
	class Compare,
	class Alloc
> bool	operator !=(
	const ft::map< Key, T, Compare, Alloc > &lhs,
	const ft::map< Key, T, Compare, Alloc > &rhs
) { return !(lhs == rhs); }

template <
	class Key,
	class T,
	class Compare,
	class Alloc
> bool	operator <=(
	const ft::map< Key, T, Compare, Alloc > &lhs,
	const ft::map< Key, T, Compare, Alloc > &rhs
) { return lhs < rhs || lhs == rhs; }

template <
	class Key,
	class T,
	class Compare,
	class Alloc
> bool	operator >(
	const ft::map< Key, T, Compare, Alloc > &lhs,
	const ft::map< Key, T, Compare, Alloc > &rhs
) { return !(lhs <= rhs); }

template <
	class Key,
	class T,
	class Compare,
	class Alloc
> bool	operator >=(
	const ft::map< Key, T, Compare, Alloc > &lhs,
	const ft::map< Key, T, Compare, Alloc > &rhs
) { return !(lhs < rhs); }
