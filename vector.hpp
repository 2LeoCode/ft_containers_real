/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 01:17:15 by crochu            #+#    #+#             */
/*   Updated: 2022/02/16 00:35:53 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <sstream>
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
				vector::const_iterator it = other.begin();

				while (it != other.end())
					m_alloc.construct(m_data++, *it++);
				m_data -= m_size;
			}

			vector(const vector &other, const Allocator &alloc)
			:	m_alloc(alloc),
				m_capacity(other.m_size),
				m_size(m_capacity),
				m_data(m_alloc.allocate(m_capacity)) {
				vector::const_iterator it = other.begin();

				while (it != other.end())
					m_alloc.construct(m_data++, *it++);
				m_data -= m_size;
			}

			~vector() {
				clear();
				m_alloc.deallocate(m_data, m_capacity);
			}

			vector	&operator =(const vector &other) {
				if (m_capacity < other.m_size) reserve(other.m_size);
				
				if (m_size < other.m_size) {
					vector::const_iterator	it = other.begin() + m_size;

					std::copy(other.begin(), other.begin() + m_size, m_data);
					while (it != other.end())
						m_alloc.construct(m_data + m_size++, *it++);
				} else {
					while (m_size > other.m_size)
						m_alloc.destroy(m_data + --m_size);
					std::copy(other.begin(), other.end(), m_data);
				}
				return *this;
			}

			void			assign(size_type count, const T &value) {
				m_assign_size(count, value);
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
			const_reference operator [](size_type pos) const
			{ return m_data[pos]; }

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

			reverse_iterator		rbegin(void)
			{ return reverse_iterator(m_data + m_size); }
			const_reverse_iterator	rbegin(void) const
			{ return const_reverse_iterator(m_data + m_size); }

			reverse_iterator		rend(void)
			{ return reverse_iterator(m_data); }
			const_reverse_iterator	rend(void) const
			{ return const_reverse_iterator(m_data); }

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

			iterator	insert(iterator pos, const T &value)
			{ return insert(pos, &value, &value + 1); }
			iterator	insert(iterator pos, size_type count, const T &value)
			{ return m_insert_size(pos, count, value); }
			template <
				class InputIt
			> iterator	insert(iterator pos, InputIt first, InputIt last) {
				typedef typename ft::is_integral< InputIt >::type	integral;
				return m_insert_dispatch(pos, first, last, integral());
			}

			iterator	erase(iterator pos) {
				std::copy(pos + 1, end(), pos);
				m_alloc.destroy(m_data + --m_size);
				return pos;
			}
			iterator	erase(iterator first, iterator last) {
				difference_type	last_to_end = std::distance(last, end());
				iterator		it = first + last_to_end;

				std::copy(last, end(), first);
				while (it != end())
					m_alloc.destroy(it++);
				m_size -= std::distance(first, last);
				return first;
			}

			void		push_back(const T &value) {
				if (!m_capacity) reserve(1);
				else if (m_size == m_capacity) reserve(m_size * 2);

				m_alloc.construct(end(), value);
				++m_size;
			}

			void		pop_back(void) { m_alloc.destroy(m_data + --m_size); }

			void		resize(size_type count, T value = T()) {
				if (count > m_capacity * 2) reserve(count);
				else if (count > m_capacity) reserve(m_capacity * 2);

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
					m_capacity = m_size = size;
					m_data = m_alloc.allocate(m_capacity);
					while (size--)
						m_alloc.construct(m_data + size, value);
				}
				template <
					class InputIt
				> void		m_init_range(InputIt first, InputIt last) {
					size_type	dist = m_capacity = m_size
								= std::distance(first, last);

					m_data = m_alloc.allocate(m_capacity);
					while (dist)
						m_alloc.construct(m_data + --dist, *--last);
				}

				void		m_assign_size(size_type size, const T &value) {
					size_type	end_fill;

					if (size > m_capacity) reserve(size);
					while (m_size > size)
						m_alloc.destroy(m_data + --m_size);
					end_fill = m_size;
					while (m_size < size)
						m_alloc.construct(m_data + m_size++, value);
					std::fill(m_data, m_data + end_fill, value);
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
					std::copy(first, last, m_data);
				}

				iterator	m_insert_size(
					iterator pos, size_type count, const T &value
				) {
					size_type		size = m_size + count;
					const size_type	new_size = size;
					const size_type	offset = pos - begin();
					iterator		new_pos;

					if (m_capacity < size) {
						if (size > 2 * m_size) reserve(size);
						else reserve(2 * m_size);
					}

					while (size > m_size)
						m_alloc.construct(m_data + --size, T());
					new_pos = begin() + offset;

					std::copy_backward(new_pos, end(), end() + count);
					std::fill(new_pos, new_pos + count, value);

					m_size = new_size;
					return new_pos;
				}

				template <
					class InputIt
				> iterator	m_insert_range(
					iterator pos, InputIt first, InputIt last
				) {
					const difference_type	count = std::distance(first, last);
					size_type				size = m_size + count;
					const size_type			new_size = size;
					const difference_type	offset = pos - m_data;
					iterator				new_pos, it;

					if (m_capacity < size) {
						if (size > 2 * m_size) reserve(size);
						else reserve(2 * m_size);
					}
					it = end();
					new_pos = m_data + offset;
					while (size > m_size && it > new_pos)
						m_alloc.construct(m_data + --size, *--it);
					while (size > m_size)
						m_alloc.construct(m_data + --size, *--last);
					while (it > new_pos)
						m_data[--size] = *--it;
					std::copy(first, last, new_pos);
					m_size = new_size;
					return new_pos;
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
					if (pos >= m_size) {
						std::ostringstream	error("vector::");
						error << __func__ << ": pos (which is "
							<< pos << ") >= this->size() (which is "
							<< m_size << ')';
						throw std::out_of_range(error.str());
					}
				}

				Allocator	m_alloc;
				size_type	m_capacity;
				size_type	m_size;
				T			*m_data;
	};

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
}

namespace std {
	template <
		class T,
		class Alloc
	> void	swap(ft::vector< T, Alloc > &lhs, ft::vector< T, Alloc > &rhs) {
		lhs.swap(rhs);
	}
}
