/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 01:17:15 by crochu            #+#    #+#             */
/*   Updated: 2022/01/27 18:13:54 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include "to_string.h"
#include "equal.hpp"
#include "lexicographical_compare.hpp"
#include "reverse_iterator.hpp"
#include "iterator_traits.hpp"
#include "is_same.hpp"
#include "is_integral.hpp"

namespace ft {
	template <
		class T,
		class Allocator = std::allocator< T >
	> class vector {
		public:
			typedef T									value_type;
			typedef Allocator							allocator_type;
			typedef std::size_t							size_type;
			typedef std::ptrdiff_t						difference_type;
			typedef value_type&							reference;
			typedef const value_type&					const_reference;
			typedef typename Allocator::pointer			pointer;
			typedef typename Allocator::const_pointer	const_pointer;
			typedef T*									iterator;
			typedef const T*							const_iterator;
			typedef ft::reverse_iterator< iterator >	reverse_iterator;
			typedef ft::reverse_iterator< const_iterator >
														const_reverse_iterator;

			vector()
			:	_allocator(),
				_capacity(0),
				_size(0),
				_array(_allocator.allocate(0)) { }

			explicit vector(const Allocator &alloc)
			:	_allocator(alloc),
				_capacity(0),
				_size(0),
				_array(_allocator.allocate(0)) { }

			vector(
				size_type size, const T &value = T(),
				const Allocator &alloc = Allocator()
			) :	_allocator(alloc),
				_capacity(size),
				_size(size),
				_array(_allocator.allocate(_capacity)) {
				while (size--)
					_allocator.construct(_array + size, value);
				std::fill(begin(), end(), value);
			}

			template <
				class InputIt
			> vector(
				InputIt a, InputIt b,
				const Allocator &alloc = Allocator()
			) :	_allocator(alloc) {
				typedef typename ft::is_integral< InputIt >::type	integral; 
				_init_dispatch(a, b, integral());
			}

			vector(const vector &other)
			:	_allocator(other._allocator),
				_capacity(other._size),
				_size(_capacity),
				_array(_allocator.allocate(_capacity)) {
				std::copy(other.begin(), other.end(), begin());
			}

			vector(const vector &other, const Allocator &alloc)
			:	_allocator(alloc),
				_capacity(other._size),
				_size(_capacity),
				_array(_allocator.allocate(_capacity)) {
				std::copy(other.begin(), other.end(), begin());
			}

			~vector() {
				while (_size--)
					_allocator.destroy(_array + _size);
				_allocator.deallocate(_array, _capacity);
			}

			vector	&operator =(const vector &other) {
				if (_capacity < other._size) reserve(other._size);

				_size = other._size;
				std::copy(other.begin(), other.end(), _array);
				return *this;
			}

			void			assign(size_type count, const T &value) {
				_assign_size(count, value);
			}
			template <
				class InputIt
			> void			assign(InputIt first, InputIt last) {
				typedef typename ft::is_integral< InputIt >::type	integral;
				_assign_dispatch(first, last, integral());
			}

			allocator_type	get_allocator(void) const { return _allocator; }

			//	Element access
			reference		at(size_type pos) {
				_range_check(pos);
				return (_array[pos]);
			}
			const_reference	at(size_type pos) const {
				_range_check(pos);
				return (_array[pos]);
			}

			reference		operator [](size_type pos) { return (_array[pos]); }
			const_reference operator [](size_type pos) const {
				return (_array[pos]);
			}

			reference		front(void) { return *_array; }
			const_reference	front(void) const { return *_array; }

			reference		back(void) { return _array[_size - 1]; }
			const_reference	back(void) const { return _array[_size - 1]; }

			T				*data(void) { return _array; }

			//	Iterators
			iterator				begin(void) { return _array; }
			const_iterator			begin(void) const { return _array; }

			iterator				end(void) { return _array + _size; }
			const_iterator			end(void) const { return _array + _size; }

			reverse_iterator		rbegin(void) { return reverse_iterator(_array + _size); }
			const_reverse_iterator	rbegin(void) const { return const_reverse_iterator(_array + _size); }

			reverse_iterator		rend(void) { return reverse_iterator(_array); }
			const_reverse_iterator	rend(void) const { return const_reverse_iterator(_array); }

			//	Capacity
			bool		empty(void) const { return !_size; }

			size_type	size(void) const { return _size; }

			size_type	max_size(void) const { return std::numeric_limits< size_type >::max() / sizeof(T); }

			void		reserve(size_type new_cap) {
				if (new_cap > max_size()) throw std::length_error(std::string("vector::") + __func__);

				if (new_cap > _capacity) {
					T	*ptr = _allocator.allocate(new_cap);
					for (iterator it = begin(); it != end(); ++it) {
						_allocator.construct(ptr++, *it);
						_allocator.destroy(it);
					}
					_allocator.deallocate(_array, _capacity);
					_capacity = new_cap;
					_array = ptr - _size;
				}
			}

			size_type	capacity(void) const { return _capacity; }

			//	Modifiers
			void		clear(void) {
				while (_size)
					_allocator.destroy(_array + --_size);
			}

			iterator	insert(iterator pos, const T &value) {
				const difference_type	offset = pos - begin();

				if (_size == _capacity) reserve(_size * 2);
				iterator				newPos = begin() + offset;

				std::copy(newPos, end(), newPos + (newPos != end()));
				*newPos = value;
				++_size;
				return newPos;
			}
			iterator	insert(iterator pos, size_type count, const T &value) {
				return _insert_size(pos, count, value);
			}
			template <
				class InputIt
			> iterator	insert(iterator pos, InputIt first, InputIt last) {
				typedef typename ft::is_integral< InputIt >::type	integral;
				return _insert_dispatch(pos, first, last, integral());
			}

			iterator	erase(iterator pos) {
				std::copy(pos + 1, end(), pos);
				--_size;
				return pos;
			}
			iterator	erase(iterator first, iterator last) {
				difference_type	range = std::distance(first, last);

				std::copy(last, end(), first);
				while (last != end())
					_allocator.destroy(last++);
				_size -= range;
				return first;
			}

			void		push_back(const T &value) {
				if (!_capacity) reserve(1);
				else if (_size == _capacity) reserve(_size * 2);

				*end() = value;
				++_size;
			}

			void		pop_back(void) { --_size; }

			void		resize(size_type count, T value = T()) {
				if (count > _capacity) reserve(count);

				while (_size > count)
					_allocator.destroy(_array + --_size);
				while (_size < count)
					_allocator.construct(_array + _size++, value);
			}

			void		swap(vector &other) {
				std::swap(_allocator, other._allocator);
				std::swap(_capacity, other._capacity);
				std::swap(_size, other._size);
				std::swap(_array, other._array);
			}


			private:

				void		_init_size(size_type size, const T &value) {
					_capacity = size;
					_size = size;
					_array = _allocator.allocate(_capacity);
					while (size--)
						_allocator.construct(_array + size, value);
				}
				template <
					class InputIt
				> void		_init_range(InputIt first, InputIt last) {
					_capacity = std::distance(first, last);
					_size = _capacity;
					_array = _allocator.allocate(_capacity);
					for (size_type i = 0; i != _size; ++i)
						_allocator.construct(_array + i, *first++);
				}

				void		_assign_size(size_type size, const T &value) {
					if (size > _capacity) reserve(size);

					while (_size > size)
						_allocator.destroy(_array + --_size);
					while (_size < size)
						_allocator.construct(_array + _size++, T());
					while (size--)
						*(_array + size) = value;
				}
				template <
					class InputIt
				> void		_assign_range(InputIt first, InputIt last) {
					size_type	size = std::distance(first, last);
					if (size > _capacity) reserve(size);

					while (_size > size)
						_allocator.destroy(_array + --_size);
					while (_size < size)
						_allocator.construct(_array + _size++, T());
					while (size--)
						*(_array + size) =  *--last;
				}

				iterator	_insert_size(iterator pos, size_type count, const T &value) {
					const size_type	size = _size + count;
					const size_type	offset = pos - begin();

					if (_capacity < size) {
						if (size > 2 * _capacity) reserve(size);
						else reserve(2 * _capacity);
					}

					size_type	i = _size;
					while (i < size)
						_allocator.construct(_array + i++, T());
					iterator		newPos = begin() + offset;

					std::copy(newPos, end(), newPos + count);
					std::fill(newPos, newPos + count, value);
					
					_size = size;
					return newPos;
				}
				template <
					class InputIt
				> iterator	_insert_range(iterator pos, InputIt first, InputIt last) {
					const difference_type	count = std::distance(first, last);
					const difference_type	offset = pos - begin();
					const size_type			size = _size + count;

					if (_capacity < size) reserve(size);
					while (_size < size)
						_allocator.construct(_array + _size++, T());
					iterator	newPos = begin() + offset;

					std::copy(newPos, end() - count, newPos + count);
					std::copy(first, last, newPos);
					_size = size;
					return newPos;
				}

				template <
					typename Integral
				> void		_init_dispatch(
					Integral size,
					const T &value,
					ft::true_type
				) { _init_size(size, value); }
				template <
					class InputIt
				> void		_init_dispatch(
					InputIt first,
					InputIt last,
					ft::false_type
				) { _init_range(first, last); }

				template <
					typename Integral
				> void		_assign_dispatch(
					Integral size,
					const T &value,
					ft::true_type
				) { _assign_size(size, value); }
				template <
					class InputIt
				> void		_assign_dispatch(
					InputIt first,
					InputIt last,
					ft::false_type
				) { _assign_range(first, last); }

				template <
					typename Integral
				> iterator	_insert_dispatch(
					iterator	pos,
					Integral	size,
					const T		&value,
					ft::true_type
				) { return _insert_size(pos, size, value); }
				template <
					class InputIt
				> iterator	_insert_dispatch(
					iterator	pos,
					InputIt		first,
					InputIt		last,
					ft::false_type
				) { return _insert_range(pos, first, last); }

				void		_range_check(size_type pos) const {
					if (pos >= _size) throw std::out_of_range(
						std::string("vector::")
						+ __func__
						+ ": pos (which is "
						+ ft::to_string(pos)
						+ ") >= this->size() (which is "
						+ ft::to_string(_size) + ')'
					);
				}

				Allocator	_allocator;
				size_type	_capacity;
				size_type	_size;
				T			*_array;
	};
}

namespace std {
	template <
		class T,
		class Alloc
	> void	swap(ft::vector< T, Alloc > &lhs, ft::vector< T, Alloc > &rhs) {
		lhs.swap(rhs);
	}
}

template <
	class T,
	class Alloc
> bool	operator ==(
	const ft::vector< T, Alloc > &lhs,
	const ft::vector< T, Alloc > &rhs
) { return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()); }

template <
	class T,
	class Alloc
> bool	operator <(
	const ft::vector< T, Alloc > &lhs,
	const ft::vector< T, Alloc > &rhs
) { return !ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

template <
	class T,
	class Alloc
> bool	operator !=(
	const ft::vector< T, Alloc > &lhs,
	const ft::vector< T, Alloc > &rhs
) { return !(lhs == rhs); }

template <
	class T,
	class Alloc
> bool	operator <=(
	const ft::vector< T, Alloc > &lhs,
	const ft::vector< T, Alloc > &rhs
) { return lhs == rhs || lhs < rhs; }

template <
	class T,
	class Alloc
> bool	operator >(
	const ft::vector< T, Alloc > &lhs,
	const ft::vector< T, Alloc > &rhs
) { return !(lhs <= rhs); }

template <
	class T,
	class Alloc
> bool	operator >=(
	const ft::vector< T, Alloc > &lhs,
	const ft::vector< T, Alloc > &rhs
) { return !(lhs < rhs); }
