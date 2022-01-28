/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsuardi <lsuardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 19:06:26 by crochu            #+#    #+#             */
/*   Updated: 2022/01/28 17:45:52 by lsuardi          ###   ########.fr       */
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
			typedef ft::pair<const Key, T>					value_type;
			typedef std::size_t								size_type;
			typedef std::ptrdiff_t							difference_type;
			typedef Compare									key_compare;
			typedef Allocator								allocator_type;
			typedef value_type&								reference;
			typedef const value_type&						const_reference;
			typedef typename Allocator::pointer				pointer;
			typedef typename Allocator::const_pointer		const_pointer;
			typedef typename rbtree<
				Key, T, value_compare, Allocator
			>::iterator										iterator;
			typedef typename rbtree<
				Key, T, value_compare, Allocator
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


			map() : _keyCmp(), _valueCmp(_keyCmp), _allocator(), _tree(_valueCmp) { }
			explicit map(
				const Compare &comp,
				const Allocator &alloc = Allocator()
			) :	_keyCmp(comp), _valueCmp(comp), _allocator(alloc), _tree(_valueCmp) { }
			template <
				class InputIt
			> map(
				InputIt first, InputIt last,
				const Compare& comp = Compare(),
				const Allocator& alloc = Allocator()
			) :	_keyCmp(comp), _valueCmp(comp), _allocator(alloc),
				_tree(first, last, _valueCmp, alloc) { }
			map(const map &other)
			:	_keyCmp(other._keyCmp),
				_valueCmp(other._keyCmp),
				_allocator(other._allocator),
				_tree(other._tree) { }
			~map() { }

			map	&operator =(const map &other) {
				_keyCmp = other._keyCmp,
				_valueCmp = other._valueCmp,
				_allocator = other._allocator,
				_tree = other._tree;
				return *this;
			}

			allocator_type	get_allocator() const { return _allocator; }

			T				&at(const Key &key) {
				return _tree.find(ft::make_pair(key, NULL)).second;
			}
			const T			&at(const Key &key) const {
				return _tree.find(ft::make_pair(key, NULL)).second;
			}
			T				&operator [](const Key &key) {
				ft::pair< iterator, bool > value;
				try {
					return _tree.find(ft::make_pair(key, NULL)).second;
				} catch (std::out_of_range &e) { }
				return _tree.insert(ft::make_pair(key, T())).first->second;
			}

			iterator				begin(void) { return _tree.begin(); }
			const_iterator			begin(void) const { return _tree.begin(); }
			iterator				end(void) { return _tree.end(); }
			const_iterator			end(void) const { return _tree.end(); }
			reverse_iterator		rbegin(void) { return _tree.rbegin(); }
			const_reverse_iterator	rbegin(void) const { return _tree.rbegin(); }
			reverse_iterator		rend(void) { return _tree.rend(); }
			const_reverse_iterator	rend(void) const { return _tree.rend(); }

			bool					empty(void) const { return !_tree.size(); }
			size_type				size(void) const { return _tree.size(); }
			size_type				max_size(void) const { return _tree.max_size(); }

			void					clear(void) { _tree.clear(); }
			ft::pair<
				iterator,
				bool
			>						insert(const value_type &value) {
				return _tree.insert(value);
			}
			ft::pair<
				iterator,
				bool
			>						insert(iterator hint, const value_type &value) {
				return _tree.insert(value, &*hint);
			}
			template <
				class InputIt
			> void					insert(InputIt first, InputIt last) {
				while (first != last)
					_tree.insert(*first++);
			}
			void					erase(iterator pos) {
				_tree.remove(pos->first, _keyCmp);
			}
			void					erase(iterator first, iterator last) {
				while (first != last) {
					erase(first++);
				}
			}
			size_type				erase(const Key &key) {
				try {
					_tree.remove(key, _keyCmp);
				} catch (...) { return 0; }
				return 1;
			}
			void					swap(map &other) {
				std::swap(_keyCmp, other._keyCmp);
				std::swap(_valueCmp, other._valueCmp);
				std::swap(_allocator, other._allocator);
				_tree.swap(other._tree);
			}

			size_type				count(const Key &key) const {
				try {
					_tree.find(ft::make_pair(key, NULL));
				} catch (...) { return 0; }
				return 1;
			}
			iterator				find(const Key &key) {
				return _tree.find_iterator(ft::make_pair(key, NULL));
			}
			const_iterator			find(const Key &key) const {
				return _tree.find_iterator(ft::make_pair(key, NULL));
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
				return find(key);
			}
			const_iterator			lower_bound(const Key &key) const {
				return find(key);
			}
			iterator				upper_bound(const Key &key) {
				iterator	it = find(key);
				if (it == end()) return it;
				return ++it;
			}
			const_iterator			upper_bound(const Key &key) const {
				const_iterator	it = find(key);
				if (it == end()) return it;
				return ++it;
			}
			
			key_compare				key_comp(void) const { return _keyCmp; }
			value_compare			value_comp(void) const { return _valueCmp; }			

			// TEST

			void					generate_dot(const char *name) const {
				_tree.generate_dot(name);
			}
			
		private:
			Compare			_keyCmp;
			value_compare	_valueCmp;
			Allocator		_allocator;
			rbtree<
				value_type,
				value_compare,
				Allocator,
				size_type,
				difference_type
			>				_tree;
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

	template <
		class Key,
		class T,
		class Compare,
		class Alloc
	> bool	operator ==(
		ft::map< Key, T, Compare, Alloc > &lhs,
		ft::map< Key, T, Compare, Alloc > &rhs
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
		ft::map< Key, T, Compare, Alloc > &lhs,
		ft::map< Key, T, Compare, Alloc > &rhs
	) { return ft::lexicographical_compare(lhs, rhs, lhs.key_comp()); }
	template <
		class Key,
		class T,
		class Compare,
		class Alloc
	> bool	operator <=(
		ft::map< Key, T, Compare, Alloc > &lhs,
		ft::map< Key, T, Compare, Alloc > &rhs
	) { return lhs < rhs || lhs == rhs; }
	template <
		class Key,
		class T,
		class Compare,
		class Alloc
	> bool	operator >(
		ft::map< Key, T, Compare, Alloc > &lhs,
		ft::map< Key, T, Compare, Alloc > &rhs
	) { return !(lhs <= rhs); }
	template <
		class Key,
		class T,
		class Compare,
		class Alloc
	> bool	operator >=(
		ft::map< Key, T, Compare, Alloc > &lhs,
		ft::map< Key, T, Compare, Alloc > &rhs
	) { return !(lhs < rhs); }
}
