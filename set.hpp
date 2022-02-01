/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 14:00:23 by Leo Suardi        #+#    #+#             */
/*   Updated: 2022/02/01 21:09:33 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pair.hpp"
#include "rbtree.hpp"
#include "reverse_iterator.hpp"
#include "lexicographical_compare.hpp"
#include "equal.hpp"

#include <functional>
#include <algorithm>
#include <cstddef>

namespace ft {
	template<
		class Key,
		class Compare = std::less< Key >,
		class Allocator = std::allocator< Key >
	> class set {
		public:
			typedef Key													key_type;
			typedef Key													value_type;
			typedef std::size_t											size_type;
			typedef std::ptrdiff_t										difference_type;
			typedef Compare												key_compare;
			typedef Compare												value_compare;
			typedef Allocator											allocator_type;
			typedef value_type&											reference;
			typedef const value_type&									const_reference;
			typedef typename Allocator::pointer							pointer;
			typedef typename Allocator::const_pointer					const_pointer;
			typedef typename rbtree<
				value_type, value_compare, Allocator, size_type, difference_type
			>::const_iterator											iterator;
			typedef iterator											const_iterator;
			typedef ft::reverse_iterator<iterator>						reverse_iterator;
			typedef ft::reverse_iterator<iterator>						const_reverse_iterator;

			set() : m_value_comp(), m_alloc(), m_tree(m_value_comp) { }
			explicit set(
				const Compare& comp,
				const Allocator& alloc = Allocator()
			) :	m_value_comp(comp),
				m_alloc(alloc),
				m_tree(comp, alloc)
			{ }

			template <
				class InputIt
			> set(
				InputIt first, InputIt last,
				const Compare& comp = Compare(),
				const Allocator& alloc = Allocator()
			) : m_value_comp(comp),
				m_alloc(alloc),
				m_tree(first, last, comp, alloc)
			{ }

			set(const set &other)
			:	m_value_comp(other.m_value_comp),
				m_alloc(other.m_alloc),
				m_tree(other.m_tree)
			{ }

			~set() { clear(); }

			set	&operator =(const set &other) {
				m_value_comp = other.m_value_comp,
				m_alloc = other.m_alloc,
				m_tree = other.m_tree;
				return *this;
			}

			allocator_type	get_allocator() const { return m_alloc; }

			iterator		begin(void) { return m_tree.begin(); }
			const_iterator	begin(void) const { return m_tree.begin(); }
			iterator		end(void) { return m_tree.end(); }
			const_iterator	end(void) const { return m_tree.end(); }

			reverse_iterator		rbegin(void) { return m_tree.rbegin(); }
			const_reverse_iterator	rbegin(void) const
			{ return m_tree.rbegin(); }
			reverse_iterator		rend(void) { return m_tree.rend(); }
			const_reverse_iterator	rend(void) const
			{ return m_tree.rend(); }

			bool		empty(void) const { return !m_tree.size(); }
			size_type	size(void) const { return m_tree.size(); }
			size_type	max_size(void) const { return m_tree.max_size(); }

			void		clear(void) { m_tree.clear(); }

			ft::pair <
				iterator,
				bool
			>			insert(const value_type &value) {
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
					m_tree.remove(key);
				} catch (...) { return 0; }
				return 1;
			}

			void					swap(set &other) {
				std::swap(m_value_comp, other.m_value_comp);
				std::swap(m_alloc, other.m_alloc);
				m_tree.swap(other.m_tree);
			}

			size_type				count(const Key &key) const {
				try {
					m_tree.find(key);
				} catch (...) { return 0; }
				return 1;
			}

			iterator				find(const Key &key) {
				try {
					return m_tree.find_iterator(key);
				} catch (...) { }
				return end();
			}
			const_iterator			find(const Key &key) const {
				try {
					return m_tree.find_iterator(key);
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
				return m_tree.lower_bound(key);
			}
			const_iterator			lower_bound(const Key &key) const {
				return m_tree.lower_bound(key);
			}
			iterator				upper_bound(const Key &key) {
				return m_tree.upper_bound(key);
			}
			const_iterator			upper_bound(const Key &key) const {
				return m_tree.upper_bound(key);
			}

			key_compare		key_comp(void) const { return m_value_comp; }
			value_compare	value_comp(void) const { return m_value_comp; }
			
		private:
			Compare		m_value_comp;
			Allocator	m_alloc;
			rbtree<
				value_type, value_compare, Allocator, size_type, difference_type
			>			m_tree;
	};

	template <
		class Key,
		class Compare,
		class Alloc
	> void	swap(
		ft::set< Key, Compare, Alloc > &lhs,
		ft::set< Key, Compare, Alloc > &rhs
	) { lhs.swap(rhs); }
}

template <
	class Key,
	class Compare,
	class Alloc
> bool	operator ==(
	const ft::set< Key, Compare, Alloc > &lhs,
	const ft::set< Key, Compare, Alloc > &rhs
) {
	return (
		lhs.size() == rhs.size()
		&& ft::equal(lhs.begin(), lhs.end(), rhs.begin())
	);
}

template <
	class Key,
	class Compare,
	class Alloc
> bool	operator <(
	const ft::set< Key, Compare, Alloc > &lhs,
	const ft::set< Key, Compare, Alloc > &rhs
) {
	return ft::lexicographical_compare(
		lhs.begin(), lhs.end(),
		rhs.begin(), rhs.end()
	);
}

template <
	class Key,
	class Compare,
	class Alloc
> bool	operator !=(
	const ft::set< Key, Compare, Alloc > &lhs,
	const ft::set< Key, Compare, Alloc > &rhs
) { return !(lhs == rhs); }

template <
	class Key,
	class Compare,
	class Alloc
> bool	operator <=(
	const ft::set< Key, Compare, Alloc > &lhs,
	const ft::set< Key, Compare, Alloc > &rhs
) { return lhs < rhs || lhs == rhs; }

template <
	class Key,
	class Compare,
	class Alloc
> bool	operator >(
	const ft::set< Key, Compare, Alloc > &lhs,
	const ft::set< Key, Compare, Alloc > &rhs
) { return !(lhs <= rhs); }

template <
	class Key,
	class Compare,
	class Alloc
> bool	operator >=(
	const ft::set< Key, Compare, Alloc > &lhs,
	const ft::set< Key, Compare, Alloc > &rhs
) { return !(lhs < rhs); }
