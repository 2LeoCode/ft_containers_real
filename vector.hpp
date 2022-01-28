/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsuardi <lsuardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 01:17:15 by crochu            #+#    #+#             */
/*   Updated: 2022/01/28 16:39:42 by lsuardi          ###   ########.fr       */
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
			:	m_alloc(),
				m_capacity(0),
				m_size(0),
				m_data(m_alloc.allocate(0)) { }

			explicit vector(const Allocator &alloc)
			:	m_alloc(alloc),
				m_capacity(0),
				m_size(0),
				m_data(m_alloc.allocate(0)) { }

			vector(
				size_type size, const T &value = T(),
				const Allocator &alloc = Allocator()
			) :	m_alloc(alloc),
				m_capacity(size),
				m_size(size),
				m_data(m_alloc.allocate(m_capacity)) {
				while (size--)
					m_alloc.construct(m_data + size, value);
				std::fill(begin(), end(), value);
			}

			template <
				class InputIt
			> vector(
				InputIt a, InputIt b,
				const Allocator &alloc = Allocator()
			) :	m_alloc(alloc) {
				typedef typename ft::is_integral< InputIt >::type	integral;
				m_init_dispatch(a, b, integral());
			}

			vector(const vector &other)
			:	m_alloc(other.m_alloc),
				m_capacity(other.m_size),
				m_size(m_capacity),
				m_data(m_alloc.allocate(m_capacity)) {
				std::copy(other.begin(), other.end(), begin());
			}

			vector(const vector &other, const Allocator &alloc)
			:	m_alloc(alloc),
				m_capacity(other.m_size),
				m_size(m_capacity),
				m_data(m_alloc.allocate(m_capacity)) {
				std::copy(other.begin(), other.end(), begin());
			}

			~vector() {
				while (m_size--)
					m_alloc.destroy(m_data + m_size);
				m_alloc.deallocate(m_data, m_capacity);
			}

			vector	&operator =(const vector &other) {
				if (m_capacity < other.m_size) reserve(other.m_size);

				m_size = other.m_size;
				std::copy(other.begin(), other.end(), m_data);
				return *this;
			}

			void			assign(size_type count, const T &value) {
				_assignm_size(count, value);
			}
			template <
				class InputIt
			> void			assign(InputIt first, InputIt last) {
				typedef typename ft::is_integral< InputIt >::type	integral;
				m_assign_dispatch(first, last, integral());
			}

			allocator_type	get_allocator(void) const { return m_alloc; }

			//	Element access
			reference		at(size_type pos) {
				m_range_check(pos);
				return (m_data[pos]);
			}
			const_reference	at(size_type pos) const {
				m_range_check(pos);
				return (m_data[pos]);
			}

			reference		operator [](size_type pos) { return m_data[pos]; }
			const_reference operator [](size_type pos) const {
				return m_data[pos];
			}

			reference		front(void) { return *m_data; }
			const_reference	front(void) const { return *m_data; }

			reference		back(void) { return m_data[m_size - 1]; }
			const_reference	back(void) const { return m_data[m_size - 1]; }

			T				*data(void) { return m_data; }

			//	Iterators
			iterator				begin(void) { return m_data; }
			const_iterator			begin(void) const { return m_data; }

			iterator				end(void) { return m_data + m_size; }
			const_iterator			end(void) const { return m_data + m_size; }

			reverse_iterator		rbegin(void) {
				return reverse_iterator(m_data + m_size);
			}
			const_reverse_iterator	rbegin(void) const {
				return const_reverse_iterator(m_data + m_size);
			}

			reverse_iterator		rend(void) {
				return reverse_iterator(m_data);
			}
			const_reverse_iterator	rend(void) const {
				return const_reverse_iterator(m_data);
			}

			//	Capacity
			bool		empty(void) const { return !m_size; }

			size_type	size(void) const { return m_size; }

			size_type	max_size(void) const {
				return (
					std::numeric_limits< size_type >::max() / (sizeof(T) * 2)
				);
			}

			void		reserve(size_type new_cap) {
				if (new_cap > max_size())
					throw std::length_error(
						std::string("vector::") + __func__
					);
				if (new_cap > m_capacity) {
					T	*ptr = m_alloc.allocate(new_cap);
					for (iterator it = begin(); it != end(); ++it) {
						m_alloc.construct(ptr++, *it);
						m_alloc.destroy(it);
					}
					m_alloc.deallocate(m_data, m_capacity);
					m_capacity = new_cap;
					m_data = ptr - m_size;
				}
			}

			size_type	capacity(void) const { return m_capacity; }

			//	Modifiers
			void		clear(void) {
				while (m_size)
					m_alloc.destroy(m_data + --m_size);
			}

			iterator	insert(iterator pos, const T &value) {
				const difference_type	offset = pos - begin();

				if (m_size == m_capacity) reserve(m_size * 2);
				iterator				newPos = begin() + offset;

				std::copy(newPos, end(), newPos + (newPos != end()));
				*newPos = value;
				++m_size;
				return newPos;
			}
			iterator	insert(iterator pos, size_type count, const T &value) {
				return m_insert_size(pos, count, value);
			}
			template <
				class InputIt
			> iterator	insert(iterator pos, InputIt first, InputIt last) {
				typedef typename ft::is_integral< InputIt >::type	integral;
				return m_insert_dispatch(pos, first, last, integral());
			}

			iterator	erase(iterator pos) {
				std::copy(pos + 1, end(), pos);
				--m_size;
				return pos;
			}
			iterator	erase(iterator first, iterator last) {
				difference_type	range = std::distance(first, last);

				std::copy(last, end(), first);
				while (last != end())
					m_alloc.destroy(last++);
				m_size -= range;
				return first;
			}

			void		push_back(const T &value) {
				if (!m_capacity) reserve(1);
				else if (m_size == m_capacity) reserve(m_size * 2);

				*end() = value;
				++m_size;
			}

			void		pop_back(void) { --m_size; }

			void		resize(size_type count, T value = T()) {
				if (count > m_capacity) reserve(count);

				while (m_size > count)
					m_alloc.destroy(m_data + --m_size);
				while (m_size < count)
					m_alloc.construct(m_data + m_size++, value);
			}

			void		swap(vector &other) {
				std::swap(m_alloc, other.m_alloc);
				std::swap(m_capacity, other.m_capacity);
				std::swap(m_size, other.m_size);
				std::swap(m_data, other.m_data);
			}


			private:

				void		m_init_size(size_type size, const T &value) {
					m_capacity = size;
					m_size = size;
					m_data = m_alloc.allocate(m_capacity);
					while (size--)
						m_alloc.construct(m_data + size, value);
				}
				template <
					class InputIt
				> void		m_init_range(InputIt first, InputIt last) {
					m_capacity = std::distance(first, last);
					m_size = m_capacity;
					m_data = m_alloc.allocate(m_capacity);
					for (size_type i = 0; i != m_size; ++i)
						m_alloc.construct(m_data + i, *first++);
				}

				void		m_assign_size(size_type size, const T &value) {
					if (size > m_capacity) reserve(size);

					while (m_size > size)
						m_alloc.destroy(m_data + --m_size);
					while (m_size < size)
						m_alloc.construct(m_data + m_size++, T());
					while (size--)
						*(m_data + size) = value;
				}
				template <
					class InputIt
				> void		m_assign_range(InputIt first, InputIt last) {
					size_type	size = std::distance(first, last);
					if (size > m_capacity) reserve(size);

					while (m_size > size)
						m_alloc.destroy(m_data + --m_size);
					while (m_size < size)
						m_alloc.construct(m_data + m_size++, T());
					while (size--)
						*(m_data + size) =  *--last;
				}

				iterator	m_insert_size(
					iterator pos, size_type count, const T &value
				) {
					const size_type	size = m_size + count;
					const size_type	offset = pos - begin();

					if (m_capacity < size) {
						if (size > 2 * m_capacity) reserve(size);
						else reserve(2 * m_capacity);
					}

					size_type	i = m_size;
					while (i < size)
						m_alloc.construct(m_data + i++, T());
					iterator		newPos = begin() + offset;

					std::copy(newPos, end(), newPos + count);
					std::fill(newPos, newPos + count, value);
					
					m_size = size;
					return newPos;
				}
				template <
					class InputIt
				> iterator	m_insert_range(
					iterator pos, InputIt first, InputIt last
				) {
					const difference_type	count = std::distance(first, last);
					const difference_type	offset = pos - begin();
					const size_type			size = m_size + count;

					if (m_capacity < size) reserve(size);
					while (m_size < size)
						m_alloc.construct(m_data + m_size++, T());
					iterator	newPos = begin() + offset;

					std::copy(newPos, end() - count, newPos + count);
					std::copy(first, last, newPos);
					m_size = size;
					return newPos;
				}

				template <
					typename Integral
				> void		m_init_dispatch(
					Integral size,
					const T &value,
					ft::true_type
				) { m_init_size(size, value); }
				template <
					class InputIt
				> void		m_init_dispatch(
					InputIt first,
					InputIt last,
					ft::false_type
				) { m_init_range(first, last); }

				template <
					typename Integral
				> void		m_assign_dispatch(
					Integral size,
					const T &value,
					ft::true_type
				) { m_assign_size(size, value); }
				template <
					class InputIt
				> void		m_assign_dispatch(
					InputIt first,
					InputIt last,
					ft::false_type
				) { m_assign_range(first, last); }

				template <
					typename Integral
				> iterator	m_insert_dispatch(
					iterator	pos,
					Integral	size,
					const T		&value,
					ft::true_type
				) { return m_insert_size(pos, size, value); }
				template <
					class InputIt
				> iterator	m_insert_dispatch(
					iterator	pos,
					InputIt		first,
					InputIt		last,
					ft::false_type
				) { return m_insert_range(pos, first, last); }

				void		m_range_check(size_type pos) const {
					if (pos >= m_size) throw std::out_of_range(
						std::string("vector::")
						+ __func__
						+ ": pos (which is "
						+ ft::to_string(pos)
						+ ") >= this->size() (which is "
						+ ft::to_string(m_size) + ')'
					);
				}

				Allocator	m_alloc;
				size_type	m_capacity;
				size_type	m_size;
				T			*m_data;
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
) {
	return (
		lhs.size() == rhs.size()
		&& ft::equal(lhs.begin(), lhs.end(), rhs.begin())
	);
}

template <
	class T,
	class Alloc
> bool	operator <(
	const ft::vector< T, Alloc > &lhs,
	const ft::vector< T, Alloc > &rhs
) {
	return ft::lexicographical_compare(
		lhs.begin(), lhs.end(), rhs.begin(), rhs.end()
	);
}

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
